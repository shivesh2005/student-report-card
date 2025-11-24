from flask import Flask, jsonify, request
import struct, os
from flask_cors import CORS

app = Flask(__name__)
CORS(app)

# Absolute path for data file
BASE_DIR = os.path.dirname(os.path.abspath(__file__))
DATA_FILE = os.path.join(BASE_DIR, "student.dat")

# Struct format: roll(int), name(50s), p,c,m,e,cs(int), per(double), grade(char)
fmt = "<i50siiiii d c"
record_size = struct.calcsize(fmt)

def read_students():
    students = []
    if not os.path.exists(DATA_FILE):
        return students
    with open(DATA_FILE, "rb") as f:
        while chunk := f.read(record_size):
            if len(chunk) != record_size:
                break
            s = struct.unpack(fmt, chunk)
            students.append({
                "rollno": s[0],
                "name": s[1].split(b"\x00")[0].decode(errors="ignore"),
                "p": s[2],
                "c": s[3],
                "m": s[4],
                "e": s[5],
                "cs": s[6],
                "per": s[7],
                "grade": s[8].decode()
            })
    return students

@app.route("/students", methods=["GET"])
def get_students():
    return jsonify(read_students())

@app.route("/students", methods=["POST"])
def add_student():
    data = request.json
    try:
        roll = int(data["roll"])
        name = data["name"]
        marks = [
            int(data["p"]),
            int(data["c"]),
            int(data["m"]),
            int(data["e"]),
            int(data["cs"])
        ]
    except Exception as e:
        return jsonify({"error": "Invalid data format", "details": str(e)}), 400

    per = sum(marks) / 5.0
    grade = b"A" if per >= 85 else b"B" if per >= 75 else b"C" if per >= 65 else b"F"

    # Pack the student record
    packed = struct.pack(
        fmt,
        roll,
        name.encode().ljust(50, b'\x00'),
        marks[0], marks[1], marks[2], marks[3], marks[4],
        per,
        grade
    )

    with open(DATA_FILE, "ab") as f:
        f.write(packed)

    return jsonify({"message": "Student added successfully."}), 201

@app.route("/students/<int:roll>", methods=["DELETE"])
def delete_student(roll):
    students = read_students()
    students = [s for s in students if s["rollno"] != roll]

    with open(DATA_FILE, "wb") as f:
        for s in students:
            packed = struct.pack(
                fmt,
                s["rollno"],
                s["name"].encode().ljust(50, b'\x00'),
                s["p"], s["c"], s["m"], s["e"], s["cs"],
                s["per"],
                s["grade"].encode()
            )
            f.write(packed)
    return jsonify({"message": "Student deleted successfully."}), 200

if __name__ == "__main__":
    app.run(debug=True)

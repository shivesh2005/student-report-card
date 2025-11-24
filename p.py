const API_URL = "http://127.0.0.1:5000";

async function fetchStudents() {
  const res = await fetch(`${API_URL}/students`);
  const data = await res.json();
  const tbody = document.querySelector("#studentsTable tbody");
  tbody.innerHTML = "";

  data.forEach(s => {
    const tr = document.createElement("tr");
    tr.innerHTML = `
      <td>${s.rollno}</td>
      <td>${s.name}</td>
      <td>${s.p}</td>
      <td>${s.c}</td>
      <td>${s.m}</td>
      <td>${s.e}</td>
      <td>${s.cs}</td>
      <td>${s.per.toFixed(2)}</td>
      <td>${s.grade}</td>
      <td>
        <button onclick="deleteStudent(${s.rollno})" class="delete-btn">🗑️ Delete</button>
      </td>
    `;
    tbody.appendChild(tr);
  });
}

async function deleteStudent(roll) {
  if (!confirm("Delete this student?")) return;
  const res = await fetch(`${API_URL}/students/${roll}`, { method: "DELETE" });
  if (res.ok) {
    alert("Student deleted.");
    fetchStudents();
  } else {
    alert("Error deleting student.");
  }
}

document.getElementById("studentForm").addEventListener("submit", async e => {
  e.preventDefault();

  const student = {
    roll: parseInt(document.getElementById("roll").value),
    name: document.getElementById("name").value,
    p: parseInt(document.getElementById("p").value),
    c: parseInt(document.getElementById("c").value),
    m: parseInt(document.getElementById("m").value),
    e: parseInt(document.getElementById("e").value),
    cs: parseInt(document.getElementById("cs").value)
  };

  const res = await fetch(`${API_URL}/students`, {
    method: "POST",
    headers: { "Content-Type": "application/json" },
    body: JSON.stringify(student)
  });

  if (res.ok) {
    alert("Student saved successfully!");
    e.target.reset();
    fetchStudents();
  } else {
    const err = await res.json();
    alert("Error adding student: " + (err.details || ""));
  }
});

fetchStudents();

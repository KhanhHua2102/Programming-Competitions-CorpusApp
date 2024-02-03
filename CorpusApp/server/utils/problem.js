const db = require("../utils/db");
const config = require("../config");

function getAllProblems({ page = 1 } = {}) {
  const offset = (page - 1) * config.listPerPage;
  const data = db.query(
    `SELECT DISTINCT *, (pi.num_solves*1.0 / pi.num_attempts*1.0) as completion_rate FROM Problem p
     LEFT JOIN ProblemInstance pi ON p.id = pi.problem_id
     GROUP BY p.name, p.author LIMIT ?,?`,
    [offset, config.listPerPage],
  );
  const meta = { page };

  return {
    data,
    meta,
  };
}

function getProblem({ id, name, author } = {}) {
  let rowData;
  if (id) {
    rowData = db.queryGet(`SELECT * FROM Problem WHERE id =?`, [id]);
  } else {
    rowData = db.queryGet(`SELECT * FROM Problem WHERE name=? AND author=?`, [
      name,
      author,
    ]);
  }
  const data = rowData ? rowData : {};
  return {
    data,
  };
}

function addProblem({ name, author, text_metadata, is_valid, optional } = {}) {
  // Check if problem already exists
  const problem = getProblem({ name, author });
  if (problem.data.id) {
    return problem.data.id;
  }

  // Add new problem
  const lastID = db.add(
    `INSERT INTO Problem (name, author, text_metadata, is_valid, optional) VALUES (?, ?, ?, ?, ?)`,
    [name, author, text_metadata, is_valid, optional],
  );
  return lastID;
}

function deleteProblem({ id } = {}) {
  console.log(`Deleting problem with ID: ${id}`);
  db.del(`DELETE FROM ProblemInstance WHERE problem_id = ?`, [id]);
  db.del(`DELETE FROM Problem WHERE id = ?`, [id]);
}

function getProblemList({ page = 1, sort = "id", order = "asc" } = {}) {
  const offset = (page - 1) * config.listPerPage;

  const data = db.query(
    `SELECT DISTINCT *, (pi.num_solves*1.0 / pi.num_attempts*1.0) as completion_rate FROM Problem p
     LEFT JOIN ProblemInstance pi ON p.id = pi.problem_id
     GROUP BY p.name, p.author
     ORDER BY ${sort} ${order} LIMIT ?,?`,
    [offset, config.listPerPage],
  );

  const meta = { page };

  return {
    data,
    meta,
  };
}

module.exports = {
  getAllProblems,
  getProblem,
  addProblem,
  deleteProblem,
  getProblemList,
};

const db = require("../utils/db");
const config = require("../config");

function getAllContests({ page = 1 } = {}) {
  const offset = (page - 1) * config.listPerPage;
  const data = db.query(`SELECT * FROM Contest LIMIT ?,?`, [
    offset,
    config.listPerPage,
  ]);
  const meta = { page };

  return {
    data,
    meta,
  };
}

function getContest({ id, name, date } = {}) {
  let rowData;
  if (id) {
    rowData = db.queryGet(`SELECT * FROM Contest WHERE id =?`, [id]);
  } else {
    rowData = db.queryGet(
      `SELECT * FROM Contest WHERE name = ? AND date = ? `,
      [name, date],
    );
  }
  const data = rowData ? rowData : {};
  return {
    data,
  };
}

function addContest({ name, description, region, date } = {}) {
  const lastID = db.add(
    `INSERT INTO Contest (name, description, region, date) VALUES (?, ?, ?, ?)`,
    [name, description, region, date],
  );
  return lastID;
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
  // console.log(`Adding problem: ${name} by ${author}`);
  const lastID = db.add(
    `INSERT INTO Problem (name, author, text_metadata, is_valid, optional) VALUES (?, ?, ?, ?, ?)`,
    [name, author, text_metadata, is_valid, optional],
  );
  return lastID;
}

function addProblemInstance({
  contest_id,
  problem_id,
  first_solve_time,
  num_solves,
  num_attempts,
  optional,
} = {}) {
  // Check if problem already exists
  const problem = db.queryGet(
    `SELECT * FROM ProblemInstance WHERE problem_id = ?`,
    [problem_id],
  );

  const problemInstance = getProblemInstance({ contest_id, problem_id });

  if (problem) {
    console.log(`Problem instance already exists`);
    // Get current metadata
    const old_num_solves = problemInstance.data.num_solves;
    const old_num_attempts = problemInstance.data.num_attempts;

    // Update problem instance metadata
    const lastID = db.add(
      `UPDATE ProblemInstance SET first_solve_time=?, num_solves=?, num_attempts=?, optional=? WHERE contest_id=? AND problem_id=?`,
      [
        first_solve_time,
        num_solves + old_num_solves,
        num_attempts + old_num_attempts,
        optional,
        contest_id,
        problem_id,
      ],
    );

    return lastID;
  }

  // console.log(`Adding problem instance`);
  const lastID = db.add(
    `INSERT INTO ProblemInstance (contest_id, problem_id, first_solve_time, num_solves, num_attempts, optional) VALUES (?, ?, ?, ?, ?, ?)`,
    [
      contest_id,
      problem_id,
      first_solve_time,
      num_solves,
      num_attempts,
      optional,
    ],
  );
  return lastID;
}

function updateProblemInstance({
  id,
  first_solve_time,
  num_solves,
  num_attempts,
  optional,
  contest_id,
  problem_id,
} = {}) {
  // console.log(`Updating problem instance with ID: ${id}`);
  // Get current metadata
  const problemInstance = getProblemInstance({ contest_id, problem_id }).data;
  const old_first_solve_time = problemInstance.first_solve_time;
  const old_num_solves = problemInstance.num_solves;
  const old_num_attempts = problemInstance.num_attempts;

  db.add(
    `
    UPDATE ProblemInstance
    SET contest_id = ?, problem_id = ?, first_solve_time = ?, num_solves = ?, num_attempts = ?, optional = ?
    WHERE id = ?
    `,
    [
      contest_id,
      problem_id,
      first_solve_time + old_first_solve_time,
      num_solves + old_num_solves,
      num_attempts + old_num_attempts,
      optional,
      id,
    ],
  );
  return id;
}

function updateContest({ id, name, description, region, date } = {}) {
  let query = "UPDATE Contest SET name = ?";
  let params = [name];

  if (description !== null) {
    query += ", description = ?";
    params.push(description);
  }

  if (region !== null) {
    query += ", region = ?";
    params.push(region);
  }

  query += ", date = ? WHERE id = ?";
  params.push(date, id);

  db.add(query, params);
  return id;
}

function getProblemInstance({ contest_id, problem_id } = {}) {
  const rowData = db.queryGet(
    `SELECT * FROM ProblemInstance WHERE contest_id =? AND problem_id = ?`,
    [contest_id, problem_id],
  );

  const data = rowData ? rowData : {};
  return {
    data,
  };
}

function updateProblem({
  id,
  name,
  author,
  text_metadata,
  is_valid,
  optional,
} = {}) {
  db.add(
    `
    UPDATE Problem
    SET name = ?, author = ?, text_metadata = ?, is_valid = ?, optional = ?
    WHERE id = ?
    `[(name, author, text_metadata, is_valid, optional, id)],
  );
  return id;
}

function deleteContest({ id } = {}) {
  db.del(`DELETE FROM ProblemInstance WHERE contest_id = ?`, [id]);
  db.del(`DELETE FROM Contest WHERE id = ?`, [id]);
}

module.exports = {
  getAllContests,
  getContest,
  addContest,
  getProblem,
  addProblem,
  addProblemInstance,
  updateProblemInstance,
  updateContest,
  updateProblem,
  getProblemInstance,
  deleteContest,
};

const db = require("../utils/db");
const config = require("../config");

function getAllProblemInstances({ page = 1 } = {}) {
  const offset = (page - 1) * config.listPerPage;
  const data = db.query(`SELECT * FROM ProblemInstance LIMIT ?,?`, [
    offset,
    config.listPerPage,
  ]);
  const meta = { page };

  return {
    data,
    meta,
  };
}

function getProblemInstance({ id, contest_id, problem_id } = {}) {
  let rowData;
  if (id) {
    rowData = db.query(`SELECT * FROM ProblemInstance WHERE id =?`, id);
  } else if (contest_id) {
    rowData = db.query(`SELECT * FROM ProblemInstance WHERE contest_id =?`, [
      contest_id,
    ]);
  } else {
    rowData = db.query(`SELECT * FROM ProblemInstance WHERE problem_id =?`, [
      problem_id,
    ]);
  }
  const data = rowData ? rowData : {};
  return {
    data,
  };
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
  const problemInstance = getProblemInstance({ contest_id, problem_id });
  if (problemInstance.data.id) {
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

  // Add new problem instance
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

// Advanced filter for problems using numerical conditions
function advancedFilterProblems({
  page = 1,
  sort = "id",
  order = "asc",
  filter,
  condition,
} = {}) {
  const offset = (page - 1) * config.listPerPage;
  if (filter == "completion_rate") {
    filter = "(num_solves*1.0 / num_attempts*1.0)";
  }

  const data = db.query(
    `SELECT DISTINCT * FROM Problem p
     LEFT JOIN ProblemInstance pi ON p.id = pi.problem_id
     WHERE ${filter} ${condition}
     GROUP BY p.name, p.author
     ORDER BY p.${sort} ${order} LIMIT ?,?`,
    [offset, config.listPerPage],
  );
  const meta = { page };

  return {
    data,
    meta,
  };
}

module.exports = {
  getAllProblemInstances,
  getProblemInstance,
  addProblemInstance,
  advancedFilterProblems,
};

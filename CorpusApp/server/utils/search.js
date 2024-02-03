const db = require("../utils/db");
const config = require("../config");

function searchProblem({ searchTerm }) {
  const data = db.query(
    `SELECT * FROM Problem WHERE LOWER(name) LIKE LOWER(?)`,
    [searchTerm],
  );
  return {
    data,
  };
}

function searchContest({ searchTerm }) {
  const data = db.query(
    `SELECT * FROM Contest WHERE LOWER(name) LIKE LOWER(?)`,
    [searchTerm],
  );
  return {
    data,
  };
}

module.exports = {
  searchContest,
  searchProblem,
};

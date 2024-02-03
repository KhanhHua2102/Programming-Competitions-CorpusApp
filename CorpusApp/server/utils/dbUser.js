require("dotenv").config();
const db = require("better-sqlite3")(process.env.NODE_ENV === "test" ? "testdbUser.db" : "dbUser.db");

function updateUser(query, ...params) {
  const stmt = db.prepare(query);
  const info = stmt.run(params);
  return info;
}

function updateUsername(query, username, id) {
  return db.prepare(query).run(username, id);
}
function updateUserPass(query, username, hash_password, id) {
  return db.prepare(query).run(username, hash_password, id);
}

function getUsers(query) {
  return db.prepare(query).all();
}
function getUserByID(query, id) {
  return db.prepare(query).get(id);
}

function createUser(query, username, password, role) {
  const result = db.prepare(query).run(username, password, role);
  return result.lastInsertRowid; // Return the ID of the inserted row
}

function deleteUserByID(query, id) {
  return db.prepare(query).run(id);
}
function deleteUserByUsername(query, username) {
  return db.prepare(query).run(username);
}

function checkIfUserExists(sql, params) {
  return db.prepare(sql).get(params);
}

module.exports = {
  updateUsername,
  updateUserPass,
  updateUser,
  getUsers,
  createUser,
  deleteUserByID,
  deleteUserByUsername,
  checkIfUserExists,
  getUserByID,
};

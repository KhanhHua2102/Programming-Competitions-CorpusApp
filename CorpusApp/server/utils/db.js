require("dotenv").config();
const sqlite = require("better-sqlite3");
const path = require("path");

const dbName = process.env.NODE_ENV === "test" ? "test.db" : "db.db";
const db = new sqlite(path.resolve(dbName), { fileMustExist: true });

function query(sql, params) {
  return db.prepare(sql).all(params);
}

function add(sql, params) {
  const result = db.prepare(sql).run(...params);
  return result.lastInsertRowid; // Return the ID of the inserted row
}

function del(sql, params) {
  db.prepare(sql).run(...params);
}

function queryGet(sql, params) {
  return db.prepare(sql).get(params);
}

function run(sql) {
  db.prepare(sql).run();
}

module.exports = {
  query,
  add,
  del,
  queryGet,
  run,
};

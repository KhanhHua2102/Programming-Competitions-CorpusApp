const request = require("supertest");
const express = require("express");
const problemRouter = require("../routes/problem");
const loginRouter = require("../routes/login");
const db = require("../utils/db");
const path = require('path');
const fs = require('fs');

async function getToken() {
  try {
    const response = await request(app).post("/login").send({
      username: 'admin',
      password: 'admin',
    });

    return response.body.token;
  } catch (error) {
    console.error(error);
  }
}
let token;


let app;

beforeAll(() => {
  app = express();
  app.use(express.json());
  app.use("/problem", problemRouter);
  app.use("/login", loginRouter);
  const sqlScript = `
    PRAGMA foreign_keys = off;

-- Drop tables if they exist
DROP TABLE IF EXISTS ProblemInstance;
DROP TABLE IF EXISTS Contest;
DROP TABLE IF EXISTS Problem;

-- Table: Contest
CREATE TABLE IF NOT EXISTS Contest (id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, name TEXT NOT NULL, description TEXT, region TEXT, date TEXT);

-- Table: Problem
CREATE TABLE IF NOT EXISTS Problem (id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, name TEXT NOT NULL, author TEXT, text_metadata TEXT, is_valid INTEGER DEFAULT (0) CHECK (is_valid IN (0, 1)), optional TEXT);

-- Table: ProblemInstance
CREATE TABLE IF NOT EXISTS ProblemInstance (id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, contest_id INTEGER REFERENCES Contest (ID) NOT NULL, problem_id INTEGER REFERENCES Problem (ID) NOT NULL, first_solve_time REAL, num_solves INTEGER, num_attempts INTEGER, optional TEXT);

PRAGMA foreign_keys = on;

-- Seeding Contest table
INSERT INTO Contest (name, description, region, date) VALUES ('Contest 1', 'This is contest 1', 'Region 1', '2023-10-01');
INSERT INTO Contest (name, description, region, date) VALUES ('Contest 2', 'This is contest 2', 'Region 2', '2023-10-02');

-- Seeding Problem table
INSERT INTO Problem (name, author, text_metadata, is_valid, optional) VALUES ('Problem 1', 'Author 1', 'Metadata 1', 1, 'Optional 1');
INSERT INTO Problem (name, author, text_metadata, is_valid, optional) VALUES ('Problem 2', 'Author 2', 'Metadata 2', 0, 'Optional 2');

-- Seeding ProblemInstance table
INSERT INTO ProblemInstance (contest_id, problem_id, first_solve_time, num_solves, num_attempts, optional) VALUES (1, 1, 1.5, 10, 20, 'Optional 1');
INSERT INTO ProblemInstance (contest_id, problem_id, first_solve_time, num_solves, num_attempts, optional) VALUES (2, 1, 2.5, 15, 25, 'Optional 2');
INSERT INTO ProblemInstance (contest_id, problem_id, first_solve_time, num_solves, num_attempts, optional) VALUES (1, 2, 3, 10, 20, 'Optional 1');
INSERT INTO ProblemInstance (contest_id, problem_id, first_solve_time, num_solves, num_attempts, optional) VALUES (2, 2, 4.5545, 15, 25, 'Optional 2');
    `;

  const sqlStatements = sqlScript.split(";");
  sqlStatements.forEach((statement) => {
    if (statement.trim() !== "") {
      db.run(statement);
    }
  });
});

beforeEach(async () => {
  token = await getToken();
  db.run("BEGIN");
});

afterEach(async () => {
  db.run("ROLLBACK");
});

describe("GET requests", () => {
  it("should get all problems", async () => {
    const res = await request(app).get("/problem").set('Authorization', `Bearer ${token}`);
    expect(res.statusCode).toEqual(200);
    expect(res.body.data).toEqual([
      {
        id: 1,
        name: 'Problem 1',
        author: 'Author 1',
        text_metadata: 'Metadata 1',
        is_valid: 1,
        optional: 'Optional 1',
        contest_id: 1,
        problem_id: 1,
        first_solve_time: 1.5,
        num_solves: 10,
        num_attempts: 20,
        completion_rate: 0.5
      },
      {
        id: 3,
        name: 'Problem 2',
        author: 'Author 2',
        text_metadata: 'Metadata 2',
        is_valid: 0,
        optional: 'Optional 1',
        contest_id: 1,
        problem_id: 2,
        first_solve_time: 3,
        num_solves: 10,
        num_attempts: 20,
        completion_rate: 0.5
      },
    ]);
  });
  it("should get a single problem", async () => {
    const res = await request(app).get("/problem/1").set('Authorization', `Bearer ${token}`);
    expect(res.statusCode).toEqual(200);
    expect(res.body.data).toEqual({
      id: 1,
      name: 'Problem 1',
      author: 'Author 1',
      text_metadata: 'Metadata 1',
      is_valid: 1,
      optional: 'Optional 1'
    });
  });
});


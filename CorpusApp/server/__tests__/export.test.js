const request = require("supertest");
const express = require("express");
const contestRouter = require("../routes/contest");
const problemRouter = require("../routes/problem");
const loginRouter = require("../routes/login");
const userRouter = require("../routes/user");
const path = require('path');
const fs = require('fs');
const db = require("../utils/db");


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
  app.use("/login", loginRouter);
  app.use("/api/contest", contestRouter);
  app.use("/api/problem", problemRouter);
  app.use("/api/user", userRouter);

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
    `;

  const sqlStatements = sqlScript.split(";");
  sqlStatements.forEach((statement) => {
    if (statement.trim() !== "") {
      db.run(statement);
    }
  });

});

// ...

afterAll(() => {
  if (process.env.NODE_ENV === 'test') {
    fs.rmdirSync(path.join(__dirname, '../test_bucket'), { recursive: true });
    fs.rmdirSync(path.join(__dirname, '../test_temp'), { recursive: true });
  }
});

beforeEach(async () => {
  token = await getToken();
  db.run("BEGIN");
});

afterEach(async () => {
  db.run("ROLLBACK");
});

describe("Corpus CRUD operations", () => {
  it("should export problems to DOMJudge", async () => {
    let res = await request(app)
      .put("/api/contest")
      .attach("file", "__tests__/contest/crud/domjudge_problems.zip")
      .set('Authorization', `Bearer ${token}`);
    expect(res.statusCode).toEqual(200);
    expect(res.text).toEqual("All items processed successfully");

    const resGet = await request(app).get("/api/problem").set('Authorization', `Bearer ${token}`);
    expect(resGet.statusCode).toEqual(200);
    const problemIds = resGet.body.data.map(problem => problem.id);    

    const resPost = await request(app)
      .post("/api/problem/export")
      .set('Authorization', `Bearer ${token}`)
      .send({
        username: 'cits3200students',
        password: 'cits3200students',
        serverUrl: 'http://170.64.171.69:12345',
        problemIds: problemIds
      });

    expect(resPost.statusCode).toEqual(200);
    expect(resPost.body.message).toEqual("Problems exported successfully");
  });  
});



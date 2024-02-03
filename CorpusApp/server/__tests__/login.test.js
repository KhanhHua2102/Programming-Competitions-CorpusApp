const request = require("supertest");
const express = require("express");
const contestRouter = require("../routes/contest");
const loginRouter = require("../routes/login");
const userRouter = require("../routes/user");
const path = require('path');
const fs = require('fs');
const db = require("../utils/db");


async function getAdminToken() {
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

async function getReadToken() {
    try {
      const response = await request(app).post("/login").send({
        username: 'readonly',
        password: 'readonly',
      });
      return response.body.token;
    } catch (error) {
      console.error(error);
    }
  }

let adminToken;
let readToken;

let app;


beforeAll(() => {
  app = express();
  app.use(express.json());
  app.use("/login", loginRouter);
  app.use("/api/contest", contestRouter);
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

// ...

afterAll(() => {
  if (process.env.NODE_ENV === 'test') {
    fs.rmdirSync(path.join(__dirname, '../test_bucket'), { recursive: true });
    fs.rmdirSync(path.join(__dirname, '../test_temp'), { recursive: true });
  }
});

beforeEach(async () => {
  adminToken = await getAdminToken();
  readToken = await getReadToken();
  db.run("BEGIN");
});

afterEach(async () => {
  db.run("ROLLBACK");
});

describe("Authorization of Corpus CRUD operations", () => {
  it("should return 200 if a full contents is added successfully", async () => {
    let res = await request(app)
      .put("/api/contest")
      .attach("file", "__tests__/contest/crud/full_new_contest.zip")
      .set('Authorization', `Bearer ${adminToken}`);
    expect(res.statusCode).toEqual(200);
    expect(res.text).toEqual("All items processed successfully");

    res = await request(app).get("/api/contest").set('Authorization', `Bearer ${adminToken}`);
    expect(res.statusCode).toEqual(200);
    expect(res.body.data).toEqual([
      {
        date: "2023-10-01",
        description: "This is contest 1",
        id: 1,
        name: "Contest 1",
        region: "Region 1",
      },
      {
        date: "2023-10-02",
        description: "This is contest 2",
        id: 2,
        name: "Contest 2",
        region: "Region 2",
      },
      {
        date: "15/09/2023",
        description: "Guys... everyone go watch the one piece live action...",
        id: 3,
        name: "Test Contest Number One.",
        region: "ANZ",
      },
    ]);
  });

  it("should return 403 if user is unauthorized", async () => {
    let res = await request(app)
      .put("/api/contest")
      .set('Authorization', `Bearer ${readToken}`);
    expect(res.statusCode).toEqual(403);
  });
  
});


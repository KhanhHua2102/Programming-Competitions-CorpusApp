const express = require("express");
const sqlite3 = require("sqlite3").verbose();
const jwt = require("jsonwebtoken");
const { hashPassword } = require("../utils/auth");
const bcrypt = require("bcrypt");

const router = express.Router();

// Connection to User db
const dbPath = process.env.NODE_ENV === "test" ? "testdbUser.db" : "dbUser.db";
const db = new sqlite3.Database(dbPath);

// Create maps to store user tokens for each role
const userTokens = new Map();

let initialLoad = true;

// POST requests from the client login page
router.post("/", (req, res) => {
  const { username, password } = req.body;
  if (!username || !password) {
    return res.json({
      message: "Please enter a username and password. All fields are required",
    });
  }

  // Use the 'db' connection to execute an SQL query
  const query = "SELECT * FROM User WHERE username = ?";
  db.get(query, [username], (err, row) => {
    if (err) {
      console.error("Error executing SQL query:", err);
      return res.status(500).json({ message: "Internal server error" });
    }

    if (!row) {
      // User not found
      res
        .status(401)
        .json({ message: "Login failed. Invalid username or password." });
      return;
    }

    let hash = JSON.parse(row.hash_password);

    async function passwordChecker(password) {
      const match = await bcrypt.compare(password, hash);
      if (!match) {
        // User not found
        res
          .status(401)
          .json({ message: "Login failed. Invalid username or password." });
        return;
      }

      // Storing user role after username found
      const userRole = row.role;

      // Check if a token already exists for this user
      let userTokenInfo = userTokens.get(username);

      if (
        !userTokenInfo ||
        Date.now() - userTokenInfo.timestamp >= 60 * 60 * 1000
      ) {
        // Generate a unique secret key for this user
        // const secretKey = crypto.randomBytes(32).toString("hex");
        const secretKey = "mysecretssshhhhhhh";

        // Generate a JWT token with a 60-minute expiration time
        const token = jwt.sign({ username, role: userRole }, secretKey, {
          expiresIn: "2h", // Token is valid for 120 minutes
        });

        // Store the token and secret key in the map along with a timestamp
        userTokenInfo = {
          token,
          secretKey,
          timestamp: Date.now(),
          role: userRole,
        };
        userTokens.set(username, userTokenInfo);
      }

      res.status(200).json({
        success: true,
        message: "Login successful",
        role: userRole,
        token: userTokenInfo.token,
      });
    }

    passwordChecker(password);
  });
});

(module.exports = router), hashPassword;

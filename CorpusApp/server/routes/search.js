const express = require("express");
const router = express.Router();
const dbService = require("../utils/search");
const multer = require("multer");
const fs = require("fs");
const { authMiddleware } = require("../utils/auth");
require("dotenv").config();

/* GET all Problems with this name. */
// can later support searching for more terms other than name
router.get("/problem", authMiddleware, (req, res) => {
  const searchTerm = `%${req.query.name}%`;
  const problemResults = dbService.searchProblem({ searchTerm });

  res.json({
    problemResults,
  });
});

/* GET all Contests with this name. */
// can later support searching for more terms other than name
router.get("/contest", authMiddleware, (req, res) => {
  const searchTerm = `%${req.query.name}%`;
  const contestResults = dbService.searchContest({ searchTerm });

  res.json({
    contestResults,
  });
});

module.exports = router;

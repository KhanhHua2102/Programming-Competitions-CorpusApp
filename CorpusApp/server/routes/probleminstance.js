const express = require("express");
const router = express.Router();
const dbService = require("../utils/probleminstance");
const multer = require("multer");
const fs = require("fs");
const { authMiddleware, authMiddlewareAdmin } = require("../utils/auth");
const { auth } = require("express-openid-connect");
require("dotenv").config();

// ####################################################################################
// Not dependendent on storage solutions (cloud storage)

/* GET all problemInstance */
router.get("/", authMiddleware, (req, res, next) => {
  try {
    res.json(dbService.getAllProblemInstances({ page: req.query.page }));
  } catch (err) {
    console.error(`Error while getting problems `, err.message);
    next(err);
  }
});

/* GET all problemInstances, given CONTEST ID */
router.get("/contest/:id", authMiddleware, (req, res, next) => {
  const { id } = req.params;
  try {
    res.json(dbService.getProblemInstance({ contest_id: id }));
  } catch (err) {
    console.error(
      `Error while getting problem information for ID: ${id}. Error: ${err.message}`,
    );
    next(err);
  }
});

/* GET all problemInstances, given PROBLEM ID */
router.get("/problem/:id", authMiddleware, (req, res, next) => {
  const { id } = req.params;
  try {
    res.json(dbService.getProblemInstance({ problem_id: id }));
  } catch (err) {
    console.error(
      `Error while getting problem information for ID: ${id}. Error: ${err.message}`,
    );
    next(err);
  }
});

/* GET a list of Problems filtered by a given column */
router.get("/filter", authMiddleware, (req, res, next) => {
  try {
    const { page, sort, order, filter, condition } = req.query;
    res.json(
      dbService.advancedFilterProblems({
        page,
        sort,
        order,
        filter,
        condition,
      }),
    );
  } catch (err) {
    console.error(`Error while getting problem list `, err.message);
    return res.status(500).send(err.message);
  }
});

/* GET a single problemInstance, given ID */
router.get("/:id", authMiddleware, (req, res, next) => {
  const { id } = req.params;
  try {
    res.json(dbService.getProblemInstance({ id }));
  } catch (err) {
    console.error(
      `Error while getting problem information for ID: ${id}. Error: ${err.message}`,
    );
    next(err);
  }
});

module.exports = router;

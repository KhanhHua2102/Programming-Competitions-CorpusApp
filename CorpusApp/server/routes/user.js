const express = require("express");
const jwt = require("jsonwebtoken");
const { authMiddlewareAdmin, authMiddleware } = require("../utils/auth");
const dbService = require("../utils/user");

const router = express.Router();

router.get("/", authMiddleware, (req, res) => {
  try {
    return res.json(dbService.getUsers());
  } catch (err) {
    console.error(`Error while getting users `, err.message);
  }
});

router.get("/:id", authMiddleware, (req, res) => {
  try {
    res.json(dbService.getUserByID(req.params.id));
  } catch (err) {
    console.error(`Error while getting user `, err.message);
  }
});

router.post("/", authMiddlewareAdmin, (req, res) => {
  try {
    res.json(dbService.createUser(req.body));
  } catch (err) {
    console.error(`Error while creating user `, err.message);
  }
});

router.put("/:id", authMiddleware, async (req, res) => {
  try {
    const result = await dbService.updateUser(req.body);
    res.json(result);
  } catch (err) {
    console.error(`Error while updating user `, err.message);
    res
      .status(500)
      .send({ error: "An error occurred while updating the user." });
  }
});

router.delete("/:id", authMiddlewareAdmin, (req, res, next) => {
  try {
    if (!parseInt(req.params.id)) {
      res.json(dbService.deleteUserByUsername(req.params.id));
    } else {
      res.json(dbService.deleteUserByID(req.params.id));
    }
  } catch (err) {
    console.error(`Error while deleting user `, err.message);
  }
});

module.exports = router;

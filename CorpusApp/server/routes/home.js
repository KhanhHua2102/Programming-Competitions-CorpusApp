const express = require("express");
const router = express.Router();
const { authMiddleware } = require("../utils/auth");

// Serve the home page
router.get("/", authMiddleware, (req, res) => {
  res.render("home"); // Assuming you have a "home" template to render
});

module.exports = router;

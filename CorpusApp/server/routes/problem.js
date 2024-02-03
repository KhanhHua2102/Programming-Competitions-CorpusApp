const express = require("express");
const router = express.Router();
const dbService = require("../utils/problem");
const fs = require("fs");
const archiver = require("archiver");
const path = require("path");
const axios = require("axios");
const FormData = require("form-data");
const { authMiddleware, authMiddlewareAdmin } = require("../utils/auth");

require("dotenv").config();

const UPLOAD_DIR = process.env.NODE_ENV === 'test' ? './test_bucket' : './bucket';

// ####################################################################################
// Not dependendent on storage solutions (cloud storage)

/* GET receive a zipped directory containing problems, given a list of IDs */
router.get("/download", authMiddleware, async (req, res) => {
  const FILES_DIR = UPLOAD_DIR;

  const ids = req.query.ids ? req.query.ids.split(",") : [];

  if (ids.length === 0) {
    return res.status(400).send("No IDs provided");
  }

  // Filter the files based on provided IDs
  const filesToZip = await Promise.all(
    ids.map(async (id) => {
      const problemData = await dbService.getProblem({ id });
      const problemName = problemData.data.name;
      return { path: `${FILES_DIR}/${id}.zip`, name: `${problemName}.zip` };
    }),
  );

  // Set the archive name
  res.attachment("files.zip");

  // Use Archiver to zip the files
  const archive = archiver("zip", {
    zlib: { level: 9 },
  });

  // Pipe the zipped data to the response
  archive.pipe(res);

  // Append the files to the archive
  filesToZip.forEach((file) => {
    archive.file(file.path, { name: file.name });
  });

  // Finish the archive
  archive.finalize();
});

/* GET all Problems */
router.get("/", authMiddleware, (req, res, next) => {
  try {
    res.json(dbService.getAllProblems({ page: req.query.page }));
  } catch (err) {
    console.error(`Error while getting problems `, err.message);
    next(err);
  }
});

/* GET a list of Problems sorted by a given column */
router.get("/list", authMiddleware, (req, res, next) => {
  const { page, sort, order } = req.query;
  try {
    res.json(dbService.getProblemList({ page, sort, order }));
  } catch (err) {
    console.error(`Error while getting problem list: `, err.message);
    next(err);
  }
});

/* GET a single Problem, given ID */
router.get("/:id", authMiddleware, (req, res, next) => {
  const { id } = req.params;
  try {
    res.json(dbService.getProblem({ id }));
  } catch (err) {
    console.error(
      `Error while getting problem information for ID: ${id}. Error: ${err.message}`,
    );
    next(err);
  }
});

/* DELETE a single Problem, given ID */
router.delete("/:id", authMiddlewareAdmin, (req, res, next) => {
  const { id } = req.params;
  try {
    // Delete the problem from the database
    const deleteResult = dbService.deleteProblem({ id });

    // Delete the corresponding file from the bucket
    const filePath = path.join(UPLOAD_DIR, `${id}.zip`);
    if (fs.existsSync(filePath)) {
      fs.unlinkSync(filePath);
    }

    res.json(deleteResult);
  } catch (err) {
    console.error(
      `Error while deleting problem with ID: ${id}. Error: ${err.message}`,
    );
    next(err);
  }
});

/* POST a list of Problems to a DOMJudge server */
router.post("/export", authMiddleware, async (req, res) => {
  const { username, password, serverUrl, contestId, problemIds } = req.body;
  // Perform the necessary operations to export the problems to DOMjudge
  try {
    for (const id of problemIds) {
      const filePath = path.join(__dirname, `.${UPLOAD_DIR}/${id}.zip`);
      const form = new FormData();
      form.append("zip", fs.createReadStream(filePath));

      const url = contestId
        ? `${serverUrl}/api/v4/contests/${contestId}/problems`
        : `${serverUrl}/api/v4/problems`;

      const auth = Buffer.from(`${username}:${password}`, "utf8").toString(
        "base64",
      );

      const response = await axios.post(url, form, {
        headers: {
          ...form.getHeaders(),
          Authorization: `Basic ${auth}`,
        },
      });

      console.log(`Problem ${id} exported successfully`);
    }

    res.json({ message: "Problems exported successfully" });
  } catch (error) {
    console.error(`Error while exporting problems: ${error.message}`);
    res.status(500).json({ error: "Failed to export problems" });
  }
});

module.exports = router;

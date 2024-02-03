const express = require("express");
const router = express.Router();
const dbService = require("../utils/contest");
const multer = require("multer");
const fs = require("fs");
const yaml = require("js-yaml");
const path = require("path");
const archiver = require("archiver");
const decompress = require("decompress");
const validateYamlFile = require("../utils/validateConfig");
const { authMiddleware, authMiddlewareAdmin } = require("../utils/auth");

const UPLOAD_DIR = process.env.NODE_ENV === 'test' ? './test_bucket' : './bucket';
const TEMP_DIR = process.env.NODE_ENV === 'test' ? './test_temp' : './temp';

// ======================
// local bucket setup
// ======================

if (!fs.existsSync(UPLOAD_DIR)) {
  fs.mkdirSync(UPLOAD_DIR);
}

if (!fs.existsSync(TEMP_DIR)) {
  fs.mkdirSync(TEMP_DIR);
}

const storage = multer.diskStorage({
  destination: (req, file, cb) => {
    cb(null, TEMP_DIR); // Set the destination of uploaded files
  },
  filename: (req, file, cb) => {
    cb(null, file.originalname); // Use the original file name as the saved name
  },
});

const upload = multer({
  storage: storage,
  // limits: {fileSize: 1024 * 1024 * 1024,},
});

// ======================
// PUT for creating/updating a contest
// ======================

/*
  This function also contains all the functionality for parsing a yaml file.
  At some point, we might want to abstract this away somewhere else so the
  code doesn't get too cluttered. (ha ha ha)
  - Accepts a zip/compressed(supports tar) file upload.
  - Checks and decompresses the uploaded file.
  - Validates and processes .yaml or .yml files found.
  - Adds contests and problems to a database, based on YAML content.
  - Moves or processes problem-related directories or files.
  - Sends appropriate responses based on the success or failure of these operations.
*/

function flattenDirectory(dir) {
  const entries = fs.readdirSync(dir, { withFileTypes: true });

  for (const entry of entries) {
    const entryPath = path.join(dir, entry.name);

    if (entry.isDirectory()) {
      flattenDirectory(entryPath); // Recursively flatten subdirectories
      const subEntries = fs.readdirSync(entryPath, { withFileTypes: true });

      for (const subEntry of subEntries) {
        const subEntryPath = path.join(entryPath, subEntry.name);
        const destPath = path.join(dir, subEntry.name);

        if (subEntry.isFile()) {
          fs.renameSync(subEntryPath, destPath); // Move files to parent directory
        }
      }

      fs.rmdirSync(entryPath); // Remove the now empty subdirectory
    }
  }
}

router.put(
  "/",
  authMiddlewareAdmin,
  upload.single("file"),
  async (req, res, next) => {
    try {
      const isEmpty = (obj) =>
        Object.keys(obj).length === 0 && obj.constructor === Object;

      if (!req.file) {
        return res.status(400).send("No file uploaded");
      }

      if (req.files && Object.keys(req.files).length > 1) {
        return res
          .status(400)
          .send(
            "Only one file is expected. Please upload a single .zip or .tar file.",
          );
      }

      const files = await decompress(req.file.path, TEMP_DIR, {
        filter: (file) =>
          file.path.endsWith(".zip") ||
          file.path.endsWith(".tar") ||
          file.path.endsWith(".gzip") ||
          file.path.endsWith(".yaml") ||
          file.path.endsWith(".yml"),
      });

      flattenDirectory(TEMP_DIR);

      if (
        !files.some(
          (file) => file.path.endsWith(".yaml") || file.path.endsWith(".yml"),
        )
      ) {
        return res
          .status(400)
          .send(
            "Error: no .yaml file in zipped directory. Did you forget to include it?",
          );
      }

      let contest_id;
      let new_contest = true; // bool for updating values
      let yamlContent;
      let contestUpload = true; // for differentiating contest vs problem upload

      // Parsing the YAML file and creating or updating the Contest row.
      for (const file of files) {
        if (file.path.endsWith(".yaml") || file.path.endsWith(".yml")) {
          try {
            yamlContent = yaml.load(file.data.toString("utf8"));
            if (validateYamlFile(yamlContent) === false) {
              return res
                .status(400)
                .send(
                  "Error: the YAML is not correctly formatted. Please refer to the documentation for correct formatting.",
                );
            }
            if (yamlContent.contest) {
              // right now, i'm using name and date as a composite key, we might want to change this for later
              const contestData = dbService.getContest({
                name: yamlContent.contest.name,
                date: yamlContent.contest.date,
              });
              if (!isEmpty(contestData.data)) {
                // if there is a contest (so we want to update)
                new_contest = false;
                contest_id = dbService.updateContest({
                  id: contestData.data["id"],
                  name: yamlContent.contest.name,
                  description: yamlContent.contest.description ?? null,
                  region: yamlContent.contest.region ?? null,
                  date: yamlContent.contest.date,
                });
              } else {
                contest_id = dbService.addContest({
                  name: yamlContent.contest.name,
                  description: yamlContent.contest.description ?? null,
                  region: yamlContent.contest.region ?? null,
                  date: yamlContent.contest.date,
                });
              }
            } else {
              contestUpload = false;
            }
          } catch (err) {
            console.error(`Error while adding contest `, err.message);
            return res
              .status(400)
              .send(
                "Error: couldn't parse YAML content. Please refer to the documentation for correct formatting.",
              );
          }
        }
      }

      if (yamlContent === null) {
        console.error(`Error: no yaml file `);
        return res
          .status(400)
          .send(
            "Error: no YAML file found. Please ensure that there is a valid YAML file in the directory.",
          );
      }
      // Function for decompressing a problem, and updating the bucket
      // and database accordingly. This assumes that the problem
      // is new, and does not exist in the database whatsoever.
      const processNewProblem = async (problem, problem_id) => {
        const directoryPath = TEMP_DIR;
        const destinationPath = UPLOAD_DIR;

        // Utility to check if file is present in directory with
        // a specified name (independent of file extension)
        const existsWithName = (directory, nameWithoutExtension) => {
          return fs
            .readdirSync(directory)
            .find(
              (item) =>
                path.basename(item, path.extname(item)) ===
                nameWithoutExtension,
            );
        };

        // Ensure that it is in fact, in the directory
        const itemNameToCheck = existsWithName(directoryPath, problem.dir_name);

        // Utility for zipping directories and moving them
        // (so we can support tars, or even normal directories, but ensure that
        // they are zipped in the bucket)
        const zipAndMove = async (sourcePath, destinationFolder) => {
          const output = fs.createWriteStream(
            path.join(
              destinationFolder,
              path.basename(sourcePath, path.extname(sourcePath)) + ".zip",
            ),
          );
          const archive = archiver("zip", {
            zlib: { level: 9 }, // Compression level
          });

          return new Promise((resolve, reject) => {
            output.on("close", () =>
              resolve(
                `File zipped and moved to ${path.join(
                  destinationFolder,
                  path.basename(sourcePath, path.extname(sourcePath)) + ".zip",
                )}`,
              ),
            );
            archive.on("error", (err) => reject(err));

            archive.pipe(output);
            if (fs.lstatSync(sourcePath).isDirectory()) {
              archive.directory(sourcePath, false);
            } else {
              archive.file(sourcePath, { name: path.basename(sourcePath) });
            }
            archive.finalize();
          });
        };

        // Main function for handling the move.
        // If it's zipped, move from temp folder to bucket.
        // If directory, or other compressed format, make it a zip, and
        // move to bucket.
        const handleItem = async (itemPath, problemName) => {
          const itemExt = path.extname(itemPath);
          const newFileName = `${problemName}${itemExt}`; // Name for the new file
          const newPath = path.join(destinationPath, newFileName);
          if (itemExt === ".zip") {
            fs.renameSync(itemPath, newPath);
          } else if ([".tar", ".tar.gz", ".rar"].includes(itemExt)) {
            await decompress(itemPath, path.dirname(itemPath));
            itemPath = path.join(
              path.dirname(itemPath),
              path.basename(itemPath, itemExt),
            );

            await zipAndMove(itemPath, newPath);
          } else {
            throw new Error(`Unrecognized format for ${itemPath}`);
          }
        };

        await handleItem(path.join(directoryPath, itemNameToCheck), problem_id);
      };

      // Remove uploaded file and temp directory when finished
      const cleanup = () => {
        try {
          fs.unlinkSync(req.file.path);
          fs.rmSync(TEMP_DIR, { recursive: true });
          fs.mkdirSync(TEMP_DIR);
        } catch (error) {
          console.error("Error cleaning up the temp directory:", error);
        }
      };

      // Run in parallel with promises so that we don't have to wait
      const promises = yamlContent.problems.map((problem) => {
        let problem_id;
        const problemData = dbService.getProblem({
          // right now, we are using name and author as composite key for problem rows.
          name: problem.name,
          author: problem.author,
        });

        // Problem rows
        if (!isEmpty(problemData.data)) {
          // if there's existing problem - so do we even need to update?
          problem_id = problemData.data["id"];
        } else {
          // if it's a new problemw
          problem_id = dbService.addProblem({
            name: problem.name,
            author: problem.author,
          });

          processNewProblem(problem, problem_id).catch(
            (error) => error.message,
          );
        }

        // ProblemInstance rows
        if (contestUpload) {
          // if there was a contest in the yaml - otherwise, it's just batch problem upload, so no need for problem instance
          const probleminstance_id = dbService.getProblemInstance({
            contest_id,
            problem_id,
          }).data.id;

          if (new_contest || probleminstance_id === undefined) {
            // to account for edge case of problems of a contest being deleted,
            // but the contest still remains
            dbService.addProblemInstance({
              contest_id,
              problem_id,
              first_solve_time: problem.first_solve_time ?? null,
              num_solves: problem.num_solves ?? null,
              num_attempts: problem.num_attempts ?? null,
              optional: problem.optional ?? null,
            });
          } else {
            dbService.updateProblemInstance({
              id: probleminstance_id,
              contest_id,
              problem_id,
              first_solve_time: problem.first_solve_time ?? null,
              num_solves: problem.num_solves ?? null,
              num_attempts: problem.num_attempts ?? null,
              optional: problem.optional ?? null,
            });
          }
        }
      });

      Promise.all(promises)
        .then((results) => {
          const errors = results.filter((result) => typeof result === "string");
          if (errors.length > 0) {
            cleanup();
            return res.status(400).send(errors.join("\n"));
          } else {
            cleanup();
            res.status(200).send("All items processed successfully");
          }
        })
        .catch((error) => {
          console.error(error);
          res.status(500).send("Server error");
        });
    } catch (err) {
      res.status(500).send("Server error.");
    }
  },
);

// ======================
// GET all contests
// ======================

router.get("/", authMiddleware, (req, res, next) => {
  try {
    res.json(dbService.getAllContests(req.query.page));
  } catch (err) {
    console.error(`Error while getting contests `, err.message);
    next(err);
  }
});

// ======================
// GET a single contest by ID
// ======================

router.get("/:id", authMiddleware, (req, res, next) => {
  const { id } = req.params;

  try {
    res.json(dbService.getContest({ id }));
  } catch (err) {
    console.error(
      `Error while getting contest information for ID: ${id}. Error: ${err.message}`,
    );
    next(err);
  }
});

// ======================
// DELETE a single contest by ID
// ======================

router.delete("/:id", authMiddlewareAdmin, (req, res, next) => {
  const { id } = req.params;
  try {
    res.json(dbService.deleteContest({ id }));
  } catch (err) {
    console.error(
      `Error while getting problem information for ID: ${id}. Error: ${err.message}`,
    );
    next(err);
  }
});

module.exports = router;

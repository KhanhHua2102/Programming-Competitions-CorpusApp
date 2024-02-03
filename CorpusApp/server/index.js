const express = require("express");
const app = express();
const { authMiddleware } = require("./utils/auth.js");

app.use(express.json()); // for parsing application/json
app.use(express.urlencoded({ extended: true })); // for parsing application

const port = 3001; // Explicitly set the port to 3001

const loginRouter = require("./routes/login");
const homeRouter = require("./routes/home");
const problemRouter = require("./routes/problem");
const contestRouter = require("./routes/contest");
const problemInstanceRouter = require("./routes/probleminstance");
const searchRouter = require("./routes/search");
const userRouter = require("./routes/user");
const cors = require("cors");
const {
  GetObjectAttributesRequestFilterSensitiveLog,
} = require("@aws-sdk/client-s3");

app.get("/", authMiddleware, (req, res) => {
  res.json({ message: "alive" });
});

const corsOptions = {
  origin: "*",
  credentials: true, // access-control-allow-credentials:true
  optionSuccessStatus: 200,
};

app.use(cors(corsOptions)); // Use this after the variable declaration

let initialLoad = true;

app.get("/clear-local-storage", (req, res) => {
  if (initialLoad) {
    res.send("localStorage.clear(); window.location.reload()");
    initialLoad = false;
  } else res.send('console.log("")');
});

// API Routes
app.use("/login", loginRouter);
app.use("/home", authMiddleware, homeRouter);
app.use("/api/contest", contestRouter);
app.use("/api/problem", problemRouter);
app.use("/api/probleminstance", problemInstanceRouter);
app.use("/api/search", searchRouter);
app.use("/api/user", userRouter);

app.listen(port, () => {
  console.log(`Example app listening at http://localhost:${port}`);
});

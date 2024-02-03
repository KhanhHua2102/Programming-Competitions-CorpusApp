const jwt = require("jsonwebtoken");
const secret = "mysecretssshhhhhhh";
const expiration = "2h";
const bcrypt = require("bcrypt");

const authMiddleware = (req, res, next) => {
  let token =
    req.headers.token || req.headers.Authorization || req.headers.authorization;
  if (req.headers.Authorization) {
    token = token.split(" ").pop().trim();
  }

  if (req.headers.authorization) {
    token = token.split(" ").pop().trim();
  }

  if (!token) {
    return res.sendStatus(401);
  }

  try {
    jwt.verify(token, secret);
    next();
  } catch (err) {
    console.log(err);

    if (err) return res.sendStatus(403);

    req.user = user;

    next();
  }
};

const authMiddlewareAdmin = (req, res, next) => {
  let token =
    req.headers.token || req.headers.Authorization || req.headers.authorization;
  if (req.headers.Authorization) {
    token = token.split(" ").pop().trim();
  }

  if (req.headers.authorization) {
    token = token.split(" ").pop().trim();
  }

  if (!token) {
    return res.sendStatus(401);
  }

  try {
    const decodedToken = jwt.verify(token, secret);
    const userRole = decodedToken.role; // assuming 'role' is a property in the token payload

    if (userRole !== "superuser" && userRole !== "admin") {
      return res.status(403).send('Forbidden'); // Forbidden
    }

    next();
  } catch (err) {
    console.log(err);

    return res.sendStatus(403).send('Error with authentication');
  }
};
const hashPassword = (password) => {
  return bcrypt.hash(password, 10);
};

module.exports = { authMiddleware, authMiddlewareAdmin, hashPassword };

const db = require("../utils/dbUser");
const { hashPassword } = require("../utils/auth");

function getUsers() {
  const data = db.getUsers(`SELECT id, username, role FROM user`);
  return {
    data,
  };
}
function getUserByID(id) {
  const data = db.getUserByID(`SELECT * FROM user WHERE id = ?`, id);
  return {
    data,
  };
}
function deleteUserByID(id) {
  const data = db.deleteUserByID(`DELETE FROM user WHERE id = ?`, id);
  return {
    data,
  };
}
function deleteUserByUsername(username) {
  const data = db.deleteUserByID(
    `DELETE FROM user WHERE username = ?`,
    username,
  );
  return {
    data,
  };
}

async function createUser(userObject) {
  let hash_password = await hashPassword(userObject.password);
  hash_password = JSON.stringify(hash_password);

  const data = db.createUser(
    `INSERT INTO user (username, hash_password, role) VALUES (?, ?, ?)`,
    userObject.username,
    hash_password,
    userObject.role,
  );
  return {
    data,
  };
}

async function updateUser(userObject) {
  // Check if at least one field is provided
  if (!userObject.username && !userObject.role && !userObject.hash_password) {
    return {
      error: "No fields provided for update",
    };
  }

  // Fetch the current role of the user
  const currentUser = db.getUserByID(
    `SELECT * FROM user WHERE id = ?`,
    userObject.id,
  );

  // Check if the current role is 'superuser'
  if (currentUser.role === "superuser") {
    return {
      error: "Cannot update the information of a superuser",
    };
  }

  // Check if username is provided and if it already exists in the database
  if (userObject.username) {
    const existingUser = await db.getUserByID(
      `SELECT * FROM user WHERE username = ?`,
      userObject.username,
    );
    if (existingUser && existingUser.id !== userObject.id) {
      return {
        error: "Username already exists",
      };
    }
  }

  // Initialize an array to hold the query parameters
  let queryParams = [];

  // Initialize the query string
  let query = `UPDATE user SET `;

  // Check if username is provided
  if (userObject.username) {
    query += `username = ?, `;
    queryParams.push(userObject.username);
  }

  // Check if role is provided
  if (userObject.role) {
    query += `role = ?, `;
    queryParams.push(userObject.role);
  }

  // Check if password is provided
  if (userObject.hash_password) {
    let hash_password = await hashPassword(userObject.hash_password);
    hash_password = JSON.stringify(hash_password);
    query += `hash_password = ?, `;
    queryParams.push(hash_password);
  }

  // Remove the last comma and space from the query string
  query = query.slice(0, -2);

  // Add the WHERE clause
  query += ` WHERE id = ?`;
  queryParams.push(userObject.id);

  // Execute the query
  const data = db.updateUser(query, ...queryParams);

  return { data };
}

module.exports = {
  getUsers,
  getUserByID,
  deleteUserByID,
  deleteUserByUsername,
  createUser,
  updateUser,
};

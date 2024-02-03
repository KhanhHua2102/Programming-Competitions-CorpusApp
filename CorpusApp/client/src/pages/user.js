import React, { useState, useEffect } from 'react';
// import { useNavigate } from 'react-router-dom';
import { UserList } from '../components/userlist';
import DOMPurify from 'dompurify';
import Auth from '../utils/auth';
import axios from 'axios';
const serverUrl = process.env.REACT_APP_SERVER_BASE_URL;

function User() {
  const [users, setUsers] = useState([]);
  const [displayAdminContent, setDisplayAdminContent] = useState(false);
  const [showConfirm, setShowConfirm] = useState(false);
  const [showNewUser, setShowNewUser] = useState(false);
  const [newUsername, setNewUsername] = useState('');
  const [newUserrole, setNewUserrole] = useState('');
  const [newPassword, setNewPassword] = useState('');
  const [confirm, setConfirmPassword] = useState('');
  const [errorMessage, setErrorMessage] = useState('');
  const [successMessage, setSuccessMessage] = useState('');

  const [updatedPassword, setUpdatedPassword] = useState('');
  const [updatedUsername, setUpdatedUsername] = useState('');
  const [confirmUpdatePassword, setConfirmUpdatePassword] = useState('');

  if (!Auth.loggedIn()) {
    window.location = '/';
  }

  Auth.isTokenExpired();
  let username = Auth.getUsername();

  useEffect(() => {
    if (Auth.getProfile() === 'superuser') {
      setDisplayAdminContent(true);
    }
    const token = Auth.getToken();

    const options = {
      method: 'GET',
      headers: {
        token: token,
      },
    };
    fetch(`${serverUrl}/api/user`, options)
      .then((response) => response.json())
      .then((data) => {
        setUsers(data.data);
      });
  }, []);

  // Function to handle delete button click
  const handleSelfDelete = (item) => {
    setShowConfirm(true);
  };

  function handleSelfConfirmDelete() {
    setErrorMessage('');
    users.forEach((user) => {
      if (user.role === 'superuser' && user.username !== Auth.getUsername()) {
        setShowConfirm(false);
        handleUserDelete(username);
        Auth.logout();
        window.location.reload(true);
      } else {
        return setErrorMessage(
          'You cannot delete your account as there are no other SuperAdmins'
        );
      }
    });
  }

  // Function to handle refresh button click
  const handleRefresh = () => {
    window.location.reload(false);
  };

  function handleUserDelete(id) {
    const token = localStorage.getItem('userToken'); //
    fetch(`${serverUrl}/api/user/${id}`, {
      method: 'DELETE',
      headers: {
        token: token,
      },
      username: Auth.getUsername(),
    })
      .then((response) => {
        if (!response.ok) {
          throw new Error(`HTTP error! status: ${response.status}`);
        }
        return response
          .text()
          .then((text) => (text.length ? JSON.parse(text) : {}));
      })
      .catch((error) => {
        console.error('There was an error!', error);
      });
  }
  async function handleSelfUpdate() {
    setErrorMessage('');

    if (updatedPassword !== confirmUpdatePassword) {
      return setErrorMessage('Passwords do not match');
    }
    if (!updatedUsername && !updatedPassword)
      return setErrorMessage('Password and Username cannot both be null');
    let adaptedPassword = updatedPassword;
    if (updatedUsername === null) adaptedPassword = '1';
    let cleanUsername = DOMPurify.sanitize(updatedUsername);

    let cleanPassword = DOMPurify.sanitize(adaptedPassword);
    let cleanID = '';
    users.forEach((user) => {
      if (user.username === Auth.getUsername()) {
        cleanID = user.id;
      }
    });
    if (!cleanUsername) cleanUsername = Auth.getUsername();
    if (!cleanPassword) cleanPassword = null;
    const token = Auth.getToken();

    const result = await axios.put(
      `${serverUrl}/api/user/:${Auth.getProfile()}`,
      {
        username: cleanUsername,
        hash_password: cleanPassword,
        id: cleanID,
      },
      {
        headers: {
          token: token,
        },
      }
    );
    console.log(result);
    if (result.data.error) {
      console.log('there was an error.', result.data.error);
      setUpdatedUsername('');
      setUpdatedPassword('');
      setConfirmUpdatePassword('');
    } else if (result.status === 200) {
      setSuccessMessage('You have successfully updated your account');
      setUpdatedUsername('');
      setUpdatedPassword('');
      setConfirmUpdatePassword('');
    }
  }

  function handleNewUser() {
    setShowNewUser(true);
  }

  async function handleCreateNewUser(event) {
    setErrorMessage('');
    try {
      if (event) {
        event.preventDefault();
      }
      if (!newPassword || !newUsername || !confirm || !newUserrole) {
        return setErrorMessage('All fields are required');
      }
      if (newPassword !== confirm) {
        return setErrorMessage('Passwords must match');
      }
      users.forEach((user) => {
        if (user.username === newUsername) {
          return setErrorMessage(
            'A user with the same username already exists'
          );
        }
      });

      setShowNewUser(false);

      const cleanUsername = DOMPurify.sanitize(newUsername);
      const cleanPassword = DOMPurify.sanitize(newPassword);
      const cleanRole = DOMPurify.sanitize(newUserrole);

      const token = localStorage.getItem('userToken');

      const response = await axios.post(
        `${serverUrl}/api/user`,
        {
          username: cleanUsername,
          password: cleanPassword,
          role: cleanRole,
        },
        {
          headers: {
            token: token,
          },
        }
      );

      if (response.status === 200) {
        setSuccessMessage('User created successfully');
      }
    } catch (err) {
      console.log(err);
    }
  }

  return (
    <div className="p-6 shadow-lg">
      {successMessage && (
        <div
          className="flex items-center p-4 mb-4 text-sm text-green-800 border border-green-300 rounded-lg bg-green-50 dark:text-green-400 dark:border-green-800"
          role="alert"
        >
          <svg
            className="flex-shrink-0 inline w-4 h-4 mr-3"
            aria-hidden="true"
            xmlns="http://www.w3.org/2000/svg"
            fill="currentColor"
            viewBox="0 0 20 20"
          >
            <path d="M10 .5a9.5 9.5 0 1 0 9.5 9.5A9.51 9.51 0 0 0 10 .5ZM9.5 4a1.5 1.5 0 1 1 0 3 1.5 1.5 0 0 1 0-3ZM12 15H8a1 1 0 0 1 0-2h1v-3H8a1 1 0 0 1 0-2h2a1 1 0 0 1 1 1v4h1a1 1 0 0 1 0 2Z" />
          </svg>
          <span className="sr-only">Info</span>
          <div className="font-bold">{successMessage}</div>
        </div>
      )}
      {showNewUser && (
        <div className="absolute inset-0 flex items-center justify-center bg-black bg-opacity-50">
          <div className="bg-white w-1/2 p-4 rounded-md flex flex-col">
            <div className="flex gap-2 flex-col text-center">
              {errorMessage && (
                <p
                  className="flex  font-bold items-center p-4 mb-4 text-l text-red-800 border border-red-300 rounded-lg bg-red-50 dark:text-red-400 dark:border-red-800"
                  role="alert"
                >
                  {' '}
                  <svg
                    className="flex-shrink-0 inline w-4 h-4 mr-3"
                    aria-hidden="true"
                    xmlns="http://www.w3.org/2000/svg"
                    fill="currentColor"
                    viewBox="0 0 20 20"
                  >
                    <path d="M10 .5a9.5 9.5 0 1 0 9.5 9.5A9.51 9.51 0 0 0 10 .5ZM9.5 4a1.5 1.5 0 1 1 0 3 1.5 1.5 0 0 1 0-3ZM12 15H8a1 1 0 0 1 0-2h1v-3H8a1 1 0 0 1 0-2h2a1 1 0 0 1 1 1v4h1a1 1 0 0 1 0 2Z" />
                  </svg>{' '}
                  {errorMessage}{' '}
                </p>
              )}
              <h1 className="font-bold text-3xl">Creating A New User....</h1>
              <form onSubmit={handleCreateNewUser}>
                <div className="flex w-full flex-wrap flex-col justify-between gap-2">
                  <h3 className="w-full font-bold text-center">Select Role:</h3>
                  <select
                    className="indent-2 w-full outline outline-2 outline-offset-0 hover:outline-4 rounded-md"
                    name="role"
                    onChange={(e) => setNewUserrole(e.target.value)}
                  >
                    <option>Select a Role</option>
                    <option value="superuser">Super Admin</option>
                    <option value="admin">Admin</option>
                    <option value="readonly">Read Only</option>
                  </select>
                </div>
                <h3 className="font-bold mt-5 text-l">Select Username:</h3>
                <input
                  className="w-full mb-5 outline outline-2 outline-offset-0 hover:outline-4 rounded-md indent-2"
                  placeholder="Username"
                  name="username"
                  onChange={(e) => setNewUsername(e.target.value)}
                ></input>
                <h3 className="font-bold text-l">Assign Password:</h3>
                <input
                  className="w-full outline mb-5 outline-2 outline-offset-0 hover:outline-4 rounded-md indent-2"
                  placeholder="Password"
                  name="password"
                  type="password"
                  onChange={(e) => setNewPassword(e.target.value)}
                ></input>
                <h3 className="font-bold text-l">Confirm Password:</h3>
                <input
                  className="w-full outline mb-5 outline-2 outline-offset-0 hover:outline-4 rounded-md indent-2"
                  placeholder="Confirm Password"
                  name="confirm-password"
                  type="password"
                  onChange={(e) => setConfirmPassword(e.target.value)}
                ></input>
                <div className="mt-4 flex justify-between">
                  <button
                    type="button"
                    onClick={() => {
                      setShowNewUser(false);
                      setErrorMessage('');
                    }}
                    className="bg-gray-500 hover:bg-gray-700 text-white font-bold py-1 px-2 rounded"
                  >
                    Go Back
                  </button>
                  <button
                    type="submit"
                    className="bg-red-500 hover:bg-red-700 text-white font-bold py-1 px-2 rounded"
                  >
                    Create User
                  </button>
                </div>
              </form>
            </div>
          </div>
        </div>
      )}
      {showConfirm && (
        <div className="absolute inset-0 flex items-center justify-center bg-black bg-opacity-50">
          <div className="bg-white p-4 rounded-md flex flex-col">
            {errorMessage && (
              <p
                className="flex  font-bold items-center p-4 mb-4 text-l text-red-800 border border-red-300 rounded-lg bg-red-50 dark:text-red-400 dark:border-red-800"
                role="alert"
              >
                {' '}
                <svg
                  className="flex-shrink-0 inline w-4 h-4 mr-3"
                  aria-hidden="true"
                  xmlns="http://www.w3.org/2000/svg"
                  fill="currentColor"
                  viewBox="0 0 20 20"
                >
                  <path d="M10 .5a9.5 9.5 0 1 0 9.5 9.5A9.51 9.51 0 0 0 10 .5ZM9.5 4a1.5 1.5 0 1 1 0 3 1.5 1.5 0 0 1 0-3ZM12 15H8a1 1 0 0 1 0-2h1v-3H8a1 1 0 0 1 0-2h2a1 1 0 0 1 1 1v4h1a1 1 0 0 1 0 2Z" />
                </svg>{' '}
                {errorMessage}{' '}
              </p>
            )}
            <div>
              <p>Are you sure you want to delete your account?</p>
              <p>
                This action is{' '}
                <span className="font-bold">not reversible.</span>
              </p>
            </div>
            <div className="mt-4 flex justify-between">
              <button
                onClick={() => {
                  setShowConfirm(false);
                  setErrorMessage('');
                }}
                className="bg-gray-500 hover:bg-gray-700 text-white font-bold py-1 px-2 rounded"
              >
                No, take me back
              </button>
              <button
                onClick={handleSelfConfirmDelete}
                className="bg-red-500 hover:bg-red-700 text-white font-bold py-1 px-2 rounded"
              >
                Yes, I'm sure
              </button>
            </div>
          </div>
        </div>
      )}
      <div className="max-w-7xl mx-auto">
        <div className="flex justify-center gap-10 items-center mb-4">
          {displayAdminContent && (
            <div className="w-3/5">
              <div className="w-full">
                {<UserList data={users} onDelete={handleUserDelete} />}
              </div>
              <div className="flex justify-center gap-10 mt-10">
                <button
                  onClick={handleNewUser}
                  className="bg-blue-500 hover:bg-blue-700 text-white font-bold py-1 px-2 rounded"
                >
                  Create New User
                </button>
                <button
                  onClick={handleRefresh}
                  className="bg-red-500 hover:bg-red-700 text-white font-bold py-1 px-2 rounded"
                >
                  Refresh Table
                </button>
              </div>
            </div>
          )}
          <div className=" w-1/3 bg-white p-4 rounded-md flex flex-col shadow-md text-center">
            {errorMessage && !showConfirm && (
              <p
                className="flex  font-bold items-center p-4 mb-4 text-l text-red-800 border border-red-300 rounded-lg bg-red-50 dark:text-red-400 dark:border-red-800"
                role="alert"
              >
                {' '}
                <svg
                  className="flex-shrink-0 inline w-4 h-4 mr-3"
                  aria-hidden="true"
                  xmlns="http://www.w3.org/2000/svg"
                  fill="currentColor"
                  viewBox="0 0 20 20"
                >
                  <path d="M10 .5a9.5 9.5 0 1 0 9.5 9.5A9.51 9.51 0 0 0 10 .5ZM9.5 4a1.5 1.5 0 1 1 0 3 1.5 1.5 0 0 1 0-3ZM12 15H8a1 1 0 0 1 0-2h1v-3H8a1 1 0 0 1 0-2h2a1 1 0 0 1 1 1v4h1a1 1 0 0 1 0 2Z" />
                </svg>{' '}
                {errorMessage}{' '}
              </p>
            )}
            {['admin', 'readonly'].includes(Auth.getProfile()) && (
              <div>
                <div className="flex gap-2 flex-col">
                  <h3 className="font-bold mt-5 text-l">Change Username:</h3>
                  <input
                    className="w-full mb-5 outline outline-2 outline-offset-0 hover:outline-4 rounded-md indent-2"
                    placeholder={Auth.getUsername()}
                    value={updatedUsername}
                    onChange={(e) => setUpdatedUsername(e.target.value)}
                  ></input>
                  <h3 className="font-bold text-l">Change Password:</h3>
                  <input
                    className="w-full outline mb-5 outline-2 outline-offset-0 hover:outline-4 rounded-md indent-2"
                    placeholder="New Password"
                    value={updatedPassword}
                    type="password"
                    onChange={(e) => setUpdatedPassword(e.target.value)}
                  ></input>
                  <h3 className="font-bold text-l">Confirm Password:</h3>
                  <input
                    className="w-full outline mb-5 outline-2 outline-offset-0 hover:outline-4 rounded-md indent-2"
                    placeholder="Confirm Password"
                    value={confirmUpdatePassword}
                    type="password"
                    onChange={(e) => setConfirmUpdatePassword(e.target.value)}
                  ></input>
                </div>
                <div className="mt-4 flex justify-between">
                  <button
                    onClick={handleSelfUpdate}
                    className="bg-blue-500 hover:bg-blue-700 text-white font-bold py-1 px-2 rounded"
                  >
                    Update my details
                  </button>
                  <button
                    onClick={() => {
                      handleSelfDelete();
                      setErrorMessage('');
                    }}
                    className="bg-red-500 hover:bg-red-700 text-white font-bold py-1 px-2 rounded"
                  >
                    Delete my account
                  </button>
                </div>
              </div>
            )}
          </div>
        </div>
      </div>
    </div>
  );
}

export default User;

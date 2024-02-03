import React, { useState } from 'react';
import { useNavigate } from 'react-router-dom';
import axios from 'axios';
import DOMPurify from 'dompurify';
import Auth from '../utils/auth';

const serverUrl = process.env.REACT_APP_SERVER_BASE_URL;

function Login() {
  const [username, setUsername] = useState('');
  const [password, setPassword] = useState('');
  const [renderedmsg, setRenderedMsg] = useState('');

  if (Auth.loggedIn()) {
    window.location = '/home';
  }

  const navigate = useNavigate();

  const handleLogin = async () => {
    try {
      // Sanitize username and password inputs to protect against XSS
      const cleanUsername = DOMPurify.sanitize(username);
      const cleanPassword = DOMPurify.sanitize(password);

      const response = await axios.post(`${serverUrl}/login`, {
        username: cleanUsername,
        password: cleanPassword,
      });

      if (response.status === 200) {
        const { success, role, token } = response.data;

        if (success) {
          localStorage.setItem('userRole', role);
          localStorage.setItem('userToken', token);
          localStorage.setItem('userName', username);

          // Redirect to home page on successful login
          navigate('/home', { state: { token } });
        } else {
          // Login failure (user error), display error message
          setRenderedMsg('Login failed. Invalid username or password.');
          console.error('Login failed');
        }
      } else {
        // Login failure (system error), display error message
        setRenderedMsg(
          response.data.message || 'Login failed. Please try again.'
        );
        console.error('Login failed');
      }
    } catch (error) {
      console.error('Error during login:', error);
      // Set the renderedmsg state in case of an error
      setRenderedMsg(
        'Login unsuccessful. Please verify your details and try again.'
      );
    }
  };

  const handleKeyPress = (event) => {
    // Check if Enter key is pressed (key code 13)
    if (event.key === 'Enter') {
      handleLogin();
    }
  };

  return (
    <div className="h-full flex flex-col justify-center items-center">
      <div className="p-6 shadow-lg w-96 rounded-md bg-white">
        <h1 className="text-2xl text-blue-600 font-bold mb-2">Log In</h1>
        <p className="text-gray-600 text-sm mb-4">
          Please log in to access the contest archive.
        </p>
        <input
          type="text"
          placeholder="Username"
          value={username}
          onChange={(e) => setUsername(e.target.value)}
          onKeyPress={handleKeyPress} // Call handleLogin on Enter key press
          className="w-full px-3 py-2 border border-gray-300 rounded-md focus:outline-none focus:ring-2 focus:ring-blue-400 focus:ring-opacity-50 mb-4"
        />
        <input
          type="password"
          placeholder="Password"
          value={password}
          onChange={(e) => setPassword(e.target.value)}
          onKeyPress={handleKeyPress} // Call handleLogin on Enter key press
          className="w-full px-3 py-2 border border-gray-300 rounded-md focus:outline-none focus:ring-2 focus:ring-blue-400 focus:ring-opacity-50 mb-4"
        />
        <button
          onClick={handleLogin}
          className="w-full px-4 py-2 bg-blue-500 hover:bg-blue-600 text-white font-bold rounded-md shadow-md transition duration-300 ease-in-out focus:outline-none focus:ring-2 focus:ring-blue-400 focus:ring-opacity-50"
        >
          Login
        </button>
        {renderedmsg && (
          <p className="text-red-600 text-sm mb-4 mt-4">{renderedmsg}</p>
        )}
      </div>
    </div>
  );
}

export default Login;

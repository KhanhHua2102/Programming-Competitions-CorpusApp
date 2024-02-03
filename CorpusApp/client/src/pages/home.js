import React, { useRef, useEffect, useState } from 'react';
import { Link, useNavigate } from 'react-router-dom';
import uploadlogo from '../images/icons8-upload-100.png';
import databaselogo from '../images/icons8-database-100.png';
import Auth from '../utils/auth';

const serverUrl = process.env.REACT_APP_SERVER_BASE_URL;

export default function Home() {
  const fileInputRef = useRef(null);
  const navigate = useNavigate();
  const [userRole, setUserRole] = useState('');
  const [token, setToken] = useState('');

  if (!Auth.loggedIn()) {
    window.location = '/';
  }

  Auth.isTokenExpired();

  useEffect(() => {
    const role = localStorage.getItem('userRole');
    const storedToken = localStorage.getItem('userToken');

    if (role) {
      setUserRole(role);
    }

    if (storedToken) {
      setToken(storedToken);
    }
  }, []);

  const handleFileChange = async (event) => {
    const file = event.target.files[0];

    if (token) {
      // User must be logged in for any functionality of the button
      if (
        file &&
        (file.type === 'application/zip' ||
          file.type === 'application/x-zip-compressed' ||
          file.name.endsWith('.zip'))
      ) {
        try {
          const formData = new FormData();
          formData.append('file', file);

          const response = await fetch(`${serverUrl}/api/contest`, {
            method: 'PUT',
            body: formData,
            headers: {
              Authorization: `Bearer ${token}`, // Include the token in the headers
            },
          });

          const responseText = await response.text();

          if (response.ok) {
            navigate(`/database`, { state: { token } }); // Pass the token to the /database route
          } else {
            if (responseText) {
              alert(responseText);
            } else {
              alert('Failed to upload file.');
            }
          }
        } catch (error) {
          console.error('There was an error uploading the file.', error);
        }
      } else {
        alert('Please select a valid ZIP file.');
      }
    } else {
      alert('Only authorized users may upload files.');
    }
  };

  const handleDivClick = () => {
    if (userRole !== 'readonly') {
      // Role check for clicking on the div
      fileInputRef.current.click();
    } else {
      alert('Only authorized users may upload files.');
    }
  };

  return (
    <div className="flex flex-col items-center justify-center bg-gray-100 flex-grow h-full">
      <div className="flex">
        <div className="w-full">
          <input
            type="file"
            ref={fileInputRef}
            style={{ display: 'none' }}
            onChange={handleFileChange}
            accept=".zip"
          />
          <div
            onClick={handleDivClick}
            className={`flex flex-col items-center bg-white p-8 m-4 rounded shadow-lg cursor-pointer transform transition-transform hover:scale-105 ${
              userRole === 'readonly' ? 'pointer-events-none opacity-50' : ''
            }`}
          >
            <h2 className="text-2xl font-bold mb-4">Upload</h2>
            <h3 className="text-xl mb-4">Files</h3>
            <div className="w-64 h-64 bg-white-200 flex items-center justify-center">
              <img src={uploadlogo} alt="Upload" />
            </div>
          </div>
        </div>
        <Link to="/database" className="w-full">
          <div className="flex flex-col items-center bg-white p-8 m-4 rounded shadow-lg cursor-pointer transform transition-transform hover:scale-105">
            <h2 className="text-2xl font-bold mb-4">Browse</h2>
            <h3 className="text-xl mb-4">Existing Problems</h3>
            <div className="w-64 h-64 bg-white-200 flex items-center justify-center">
              <img src={databaselogo} alt="Database" />
            </div>
          </div>
        </Link>
      </div>
    </div>
  );
}

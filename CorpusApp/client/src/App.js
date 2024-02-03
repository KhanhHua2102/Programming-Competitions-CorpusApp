import React, { useEffect, useState } from 'react';
import ExportProblemsModal from './components/exportProblemsModal';
import { Route, Routes } from 'react-router-dom';
import Navbar from './components/navbar';

import Login from './pages/login';
import Logout from './pages/logout';
import Home from './pages/home';
import Database from './pages/database';
import Contest from './pages/contest';
import ProblemDetail from './pages/problemDetail';
import ContestDetail from './pages/contestDetail';
import User from './pages/user';
import { ExportProblemsProvider } from './context/exportProblemsContext';

const axios = require('axios').default;

const serverUrl = process.env.REACT_APP_SERVER_BASE_URL;

const App = () => {
  const [isModalOpen, setModalOpen] = useState(false);
  const [userRole, setUserRole] = useState('');

  useEffect(() => {
    fetch(`${serverUrl}/clear-local-storage`)
      .then((response) => response.text())
      .then((data) => eval(data))
      .catch((error) => console.error('Error clearing local storage:', error));
  }, []);

  useEffect(() => {
    // Retrieve the 'role' from local storage
    const role = localStorage.getItem('userRole');
    if (role) {
      setUserRole(role);
    }
  }, []);

  // Initialize the data you want to share between components.
  const sharedData = {
    totalSolves: 0, // Or any initial value you see fit
    // Add other shared state/data if needed
  };

  return (
    <ExportProblemsProvider>
      <div className="flex flex-col h-screen ">
        <Navbar
          isModalOpen={isModalOpen}
          setModalOpen={setModalOpen}
          userRole={userRole}
        />
        <div className="flex flex-grow overflow-hidden">
          <div className="flex-grow bg-white overflow-auto">
            <Routes>
              <Route path="/" element={<Login />} />
              <Route path="/*" element={<Login />} />
              <Route path="/user" element={<User />} />
              <Route path="/home" element={<Home userRole={userRole} />} />
              <Route
                path="/database"
                element={<Database userRole={userRole} />}
              />
              <Route
                path="/database/:id"
                element={<ProblemDetail userRole={userRole} />}
              />
              <Route
                path="/contest"
                element={<Contest userRole={userRole} />}
              />
              <Route
                path="/contest/:id"
                element={<ContestDetail userRole={userRole} />}
              />
              <Route path="/logout" element={<Logout />} />
            </Routes>
          </div>
          <ExportProblemsModal isOpen={isModalOpen} />
        </div>
      </div>
    </ExportProblemsProvider>
  );
};

export default App;

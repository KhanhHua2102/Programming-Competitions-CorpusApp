import React from 'react';
import { Link, useLocation } from 'react-router-dom';
import Auth from '../utils/auth';

const Navbar = ({ isModalOpen, setModalOpen }) => {
  const location = useLocation();

  //Check if current location is base route '/'
  const isBaseRoute = location.pathname === '/';

  //Use userRole from current route if available
  const userRole = Auth.getProfile();

  const routeChange = (location) => {
    window.location = location;
  };

  return (
    <nav className="bg-blue-600 p-4 text-white sticky top-0 w-full z-10">
      <div className="container mx-auto flex flex-wrap justify-between items-center">
        <div className="flex flex-col space-y-2">
          <div className="flex items-center space-x-2">
            <span className="text-xl font-semibold">
              Contest Problem Archive
            </span>
          </div>
          {userRole && !isBaseRoute && (
            <div className="bg-white text-black rounded-md px-2 py-1">
              {`User Role: ${userRole}`}
            </div>
          )}
        </div>

        <div className="flex flex-col md:flex-row items-center justify-center md:space-x-4 flex-grow flex-wrap">
          {!isBaseRoute && userRole && (
            <>
              <Link to={'/home'} className="hover:text-gray-300 m-1">
                Homepage
              </Link>
              <Link to={'/database'} className="hover:text-gray-300 m-1">
                Problem Database
              </Link>
              <Link to={'/contest'} className="hover:text-gray-300 m-1">
                Contests
              </Link>
              <a
                href="https://github.com/bibyen/cits3200-project"
                className="hover:text-gray-300 m-1"
                target="_blank"
                rel="noopener noreferrer"
              >
                Documentation
              </a>
              <div className="flex flex-col md:flex-row md:space-x-3 space-y-1 md:space-y-0 mt-2 md:mt-0">
                <button
                  className="bg-blue-800 px-4 py-2 rounded hover:text-gray-300 m-1"
                  onClick={() => setModalOpen(!isModalOpen)}
                >
                  {isModalOpen ? 'Minimize' : 'Open Selected Problems'}
                </button>
                <button
                  className="bg-blue-800 px-4 py-2 rounded hover:text-gray-300 m-1"
                  onClick={() => {
                    routeChange('/logout');
                  }}
                >
                  {'Logout'}
                </button>
                <Link
                  to={'/user'}
                  className="bg-blue-800 px-4 py-2 rounded hover:text-gray-300 m-1"
                >
                  {userRole === 'superuser' ? (
                    <p>Edit Users</p>
                  ) : (
                    <p>Edit Profile</p>
                  )}
                </Link>
              </div>
            </>
          )}
        </div>
      </div>
    </nav>
  );
};

export default Navbar;

// Import everything needed to use the `useQuery` hook
import { ContestList } from '../components/contestList';
import { useState, useEffect } from 'react';
import Auth from '../utils/auth';
const serverUrl = process.env.REACT_APP_SERVER_BASE_URL;

// we need to a) display info about a contest, and b) display all contests that that contest includes

export default function Contest() {
  const [selectedContests, setSelectedContests] = useState([]);
  const [contests, setContests] = useState([]);
  const [searchTerm, setSearchTerm] = useState('');
  const userRole = Auth.getUserRole();

  if (!Auth.loggedIn()) {
    window.location = '/';
  }

  Auth.isTokenExpired();

  useEffect(() => {
    const token = Auth.getToken();
    const options = {
      headers: {
        token: token,
      },
    };
    fetch(`${serverUrl}/api/contest/`, options)
      .then((response) => response.json())
      .then((data) => {
        setContests(data.data);
      });
  }, []);

  const handleSelectionChange = (id) => {
    if (selectedContests.includes(id)) {
      // deselecting the row
      setSelectedContests((prev) => prev.filter((rowId) => rowId !== id));
    } else {
      // selecting the row
      setSelectedContests((prev) => [...prev, id]);
    }
  };

  const handleSearchChange = (event) => {
    setSearchTerm(event.target.value);
    const token = Auth.getToken();

    const options = {
      headers: {
        token: token,
      },
    };
    fetch(`${serverUrl}/api/search/contest?name=${event.target.value}`, options)
      .then((response) => response.json())
      .then((data) => {
        setContests(data.contestResults.data);
      });
  };

  const handleContestDelete = (id) => {
    const token = Auth.getToken();

    fetch(`${serverUrl}/api/contest/${id}`, {
      method: 'DELETE',
      headers: {
        token: token,
      },
    })
      .then((response) => {
        if (!response.ok) {
          throw new Error(`HTTP error! status: ${response.status}`);
        }
        return response
          .text()
          .then((text) => (text.length ? JSON.parse(text) : {}));
      })
      .then((data) => {
        const updatedContests = contests.filter((contest) => contest.id !== id);
        setContests(updatedContests);
      })
      .catch((error) => {
        console.error('There was an error!', error);
      });
  };

  return (
    <div className="p-6 shadow-lg">
      <div className="max-w-7xl mx-auto">
        <div className="flex justify-between items-center mb-4">
          <input
            type="text"
            placeholder="Search for a contest..."
            className="flex-grow px-3 py-2 border border-gray-300 rounded-md focus:outline-none focus:ring-2 focus:ring-blue-400 focus:ring-opacity-50"
            value={searchTerm}
            onChange={handleSearchChange}
          />
        </div>
        <ContestList
          data={contests}
          onRowClick={handleSelectionChange}
          selectedRows={selectedContests}
          onDelete={handleContestDelete}
          userRole={userRole}
        />
      </div>
    </div>
  );
}

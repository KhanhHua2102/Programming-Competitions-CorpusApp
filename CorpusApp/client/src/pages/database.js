import { ProblemList } from '../components/problemList';
import { useState, useEffect } from 'react';
import { useExportProblems } from '../context/exportProblemsContext';
import React, { useContext } from 'react';
import DataContext from '../context/DataContext';
import Auth from '../utils/auth';
const serverUrl = process.env.REACT_APP_SERVER_BASE_URL;

export default function Database() {
  // const { loading, error, data } = useQuery(GET_ALL_PROBLEMS);
  const [selectedProblems, setSelectedProblems] = useState([]);
  const [database, setDatabase] = useState([]);
  const [exportProblems, setExportProblems] = useExportProblems();
  const [searchTerm, setSearchTerm] = useState('');
  const context = useContext(DataContext);
  const userRole = Auth.getUserRole();

  if (!Auth.loggedIn()) {
    window.location = '/';
  }
  Auth.isTokenExpired();

  const token = Auth.getToken();

  const options = {
    headers: {
      token: token,
    },
  };
  useEffect(() => {
    fetch(`${serverUrl}/api/problem`, options)
      .then((response) => response.json())
      .then((data) => {
        const problemInstances = data.data;

        return mergeData(problemInstances);
      })
      .then((mergedData) => {
        setDatabase(mergedData);
      });
  }, []);

  const mergeData = (problemInstances) => {
    return fetch(`${serverUrl}/api/problem`, options)
      .then((response) => response.json())
      .then((data) => {
        const problems = data.data;
        const problemMap = new Map(
          problems.map((problem) => [String(problem.problem_id), problem])
        );

        return problemInstances.map((instance) => {
          const problem = problemMap.get(String(instance.problem_id));
          return {
            ...instance,
            name: problem ? problem.name : 'Unknown',
          };
        });
      });
  };

  const handleSelectionChange = (item) => {
    const problem_id = item.problem_id;
    if (selectedProblems.find((problem) => problem.problem_id === problem_id)) {
      // deselecting the row
      setSelectedProblems((prev) =>
        prev.filter((problem) => problem.problem_id !== problem_id)
      );
    } else {
      // selecting the row
      setSelectedProblems((prev) => [...prev, item]);
    }
  };

  const sendToExportProblems = () => {
    const appendToList = (newObj) => {
      if (!exportProblems.some((obj) => obj.id === newObj.id)) {
        setExportProblems((prevList) => [...prevList, newObj]);
      }
    };

    selectedProblems.forEach((problem) => {
      const {
        problem_id: id,
        name,
        author,
        text_metadata,
        is_valid,
        optional,
      } = problem;
      const newProblem = {
        id,
        name,
        author,
        text_metadata,
        is_valid,
        optional,
      };

      appendToList(newProblem);
    });

    setSelectedProblems([]);
  };

  const handleSearchChange = (event) => {
    setSearchTerm(event.target.value);

    if (event.target.value.trim() === '') {
      // Fetch initial problem instances when search bar is empty
      fetchInitialData();
      return;
    }

    const idPattern = /\[id=(\d+)\]/;
    const idMatch = event.target.value.match(idPattern);

    const filterPattern = /\[(.+)([<>=]+)(\d+(\.\d+)?)\]/;
    const filterMatch = event.target.value.match(filterPattern);

    if (filterMatch) {
      const [, filter, operator, value] = filterMatch;
      fetch(
        `${serverUrl}/api/probleminstance/filter?filter=${filter}&condition=${operator}${value}`,
        options
      )
        .then((response) => response.json())
        .then((data) => {
          return mergeData(data.data);
        })
        .then((mergedData) => {
          setDatabase(mergedData);
        })
        .catch((error) => {
          console.error('There was an error!', error);
        });
    } else if (idMatch) {
      const [, id] = idMatch;
      fetch(`${serverUrl}/api/probleminstance/${id}`, options)
        .then((response) => response.json())
        .then((data) => {
          return mergeData([data.data]);
        })
        .then((mergedData) => {
          setDatabase(mergedData);
        })
        .catch((error) => {
          console.error('There was an error!', error);
        });
    } else {
      // Client-side search for non-database searches
      const lowerCaseSearchTerm = event.target.value.toLowerCase();
      const filteredData = database.filter((item) =>
        Object.values(item).some(
          (value) =>
            value &&
            value.toString().toLowerCase().includes(lowerCaseSearchTerm)
        )
      );

      setDatabase(filteredData);
    }
  };

  const fetchInitialData = () => {
    fetch(`${serverUrl}/api/probleminstance`, options) // Note: options is used here.
      .then((response) => response.json())
      .then((data) => {
        const problemInstances = data.data;
        return mergeData(problemInstances);
      })
      .then((mergedData) => {
        setDatabase(mergedData);
      })
      .catch((error) => {
        console.error('There was an error!', error);
      });
  };

  const handleProblemDelete = (id) => {
    fetch(`${serverUrl}/api/problem/${id}`, {
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
        // Remove the deleted problem from exportProblems and the database
        const updatedExportProblems = exportProblems.filter(
          (problem) => problem.problem_id !== id
        );
        setExportProblems(updatedExportProblems);
        const updatedDatabase = database.filter(
          (problem) => problem.problem_id !== id
        );
        setDatabase(updatedDatabase);
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
            placeholder="Search for a problem..."
            className="flex-grow px-3 py-2 border border-gray-300 rounded-md focus:outline-none focus:ring-2 focus:ring-blue-400 focus:ring-opacity-50"
            value={searchTerm}
            onChange={handleSearchChange}
          />
          <button
            onClick={sendToExportProblems}
            className="ml-4 px-4 py-2 bg-blue-500 hover:bg-blue-600 text-white rounded shadow-md transition duration-300 ease-in-out focus:outline-none focus:ring-2 focus:ring-blue-400 focus:ring-opacity-50"
          >
            Add Selected Problems to Export List
          </button>
        </div>
        <ProblemList
          data={database}
          onRowClick={handleSelectionChange}
          selectedRows={selectedProblems}
          onDelete={handleProblemDelete}
          userRole={userRole} // Pass userRole to ProblemList
        />
      </div>
    </div>
  );
}

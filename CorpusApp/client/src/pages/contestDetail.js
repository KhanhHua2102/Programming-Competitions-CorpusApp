import { useParams, useNavigate } from 'react-router-dom';
import { useEffect, useState } from 'react';
import { useExportProblems } from '../context/exportProblemsContext';
import React, { useContext } from 'react';
import Auth from '../utils/auth';
const serverUrl = process.env.REACT_APP_SERVER_BASE_URL;

function ContestDetail() {
  const navigate = useNavigate();
  const [contestData, setContestData] = useState(null);
  const [problemInstanceData, setProblemInstanceData] = useState(null);
  const [selectedProblems, setSelectedProblems] = useState([]);
  const [exportProblems, setExportProblems] = useExportProblems();
  const [sortConfig, setSortConfig] = useState({
    key: null,
    direction: 'none',
  });

  let { id } = useParams();

  const onSort = (key) => {
    let direction = 'ascending';
    if (sortConfig.key === key && sortConfig.direction === 'ascending') {
      direction = 'descending';
    } else if (
      sortConfig.key === key &&
      sortConfig.direction === 'descending'
    ) {
      direction = 'none';
    }

    setSortConfig({ key, direction });
  };
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
    fetch(`${serverUrl}/api/contest/${id}`, options)
      .then((response) => response.json())
      .then((data) => {
        setContestData(data.data);
      });

    fetch(`${serverUrl}/api/probleminstance/contest/${id}`, options)
      .then((response) => response.json())
      .then((data) => {
        const promises = data.data.map((problemInstance) =>
          fetch(
            `${serverUrl}/api/problem/${problemInstance.problem_id}`,
            options
          )
            .then((response) => response.json())
            .then((problemData) => ({
              ...problemInstance,
              problem_name: problemData.data.name,
            }))
        );
        Promise.all(promises).then(setProblemInstanceData);
      });
  }, [id]);

  const handleSelectionChange = (item) => {
    const token = Auth.getToken();

    const options = {
      headers: {
        token: token,
      },
    };
    const id = item.problem_id;
    if (selectedProblems.find((problem) => problem.id === id)) {
      // deselecting the row
      setSelectedProblems((prev) =>
        prev.filter((problem) => problem.id !== id)
      );
    } else {
      // selecting the row
      fetch(`${serverUrl}/api/problem/${item.problem_id}`, options)
        .then((response) => response.json())
        .then((data) => {
          setSelectedProblems((prev) => [...prev, data.data]);
        });
    }
  };

  const sendToExportProblems = () => {
    const appendToList = (newObj) => {
      if (!exportProblems.some((obj) => obj.id === newObj.id)) {
        setExportProblems((prevList) => [...prevList, newObj]);
      }
    };

    selectedProblems.forEach((problem) => {
      const { problem_id, name, author, text_metadata, is_valid, optional } =
        problem;
      const newProblem = {
        id: problem,
        name,
        author,
        text_metadata,
        is_valid,
        optional,
      };

      appendToList(problem);
    });

    setSelectedProblems([]);
  };

  let sortedProblems = [...(problemInstanceData || [])];

  if (sortConfig.key !== null) {
    sortedProblems.sort((a, b) => {
      if (a[sortConfig.key] < b[sortConfig.key]) {
        return sortConfig.direction === 'ascending' ? -1 : 1;
      }
      if (a[sortConfig.key] > b[sortConfig.key]) {
        return sortConfig.direction === 'ascending' ? 1 : -1;
      }
      return 0;
    });
  }

  return (
    <div
      style={{ minHeight: 'calc(100vh - 80px)' }}
      className="h-full min-h-screen flex flex-col max-w-3xl mx-auto bg-white p-6 rounded-lg shadow-md"
    >
      {/* Contest Details */}
      <div className="flex-grow overflow-auto">
        <h1 className="text-2xl font-semibold mb-3">
          {contestData && contestData.name}
        </h1>

        <div className="mb-4">
          <h2 className="text-xl font-medium">Contest ID:</h2>
          <p className="mt-2 text-gray-600">{contestData && contestData.id}</p>
        </div>

        <div className="mb-4">
          <h2 className="text-xl font-medium">Date:</h2>
          <p className="mt-2 text-gray-600">
            {contestData && contestData.date}
          </p>
        </div>

        <div className="mb-4">
          <h2 className="text-xl font-medium">Description:</h2>
          <p className="mt-2 text-gray-600">
            {contestData && contestData.description}
          </p>
        </div>

        <div className="mb-4">
          <h2 className="text-xl font-medium">Region:</h2>
          <p className="mt-2 text-gray-600">
            {contestData && contestData.region}
          </p>
        </div>

        <div>
          <h2 className="text-xl font-medium">Problems:</h2>
          {problemInstanceData && (
            <div>
              <table className="min-w-full divide-y divide-gray-200">
                <thead className="bg-gray-50">
                  <tr>
                    <th
                      className="px-6 py-3 text-left text-xs font-medium text-gray-500 uppercase tracking-wider cursor-pointer"
                      onClick={() => onSort('problem_name')}
                    >
                      Problem Name
                    </th>
                    <th
                      className="px-6 py-3 text-left text-xs font-medium text-gray-500 uppercase tracking-wider cursor-pointer"
                      onClick={() => onSort('num_attempts')}
                    >
                      Number of Attempts
                    </th>
                    <th
                      className="px-6 py-3 text-left text-xs font-medium text-gray-500 uppercase tracking-wider cursor-pointer"
                      onClick={() => onSort('num_solves')}
                    >
                      Number of Solves
                    </th>
                    <th
                      className="px-6 py-3 text-left text-xs font-medium text-gray-500 uppercase tracking-wider cursor-pointer"
                      onClick={() => onSort('first_solve_time')}
                    >
                      Time of First Solve
                    </th>
                  </tr>
                </thead>
                <tbody className="bg-white divide-y divide-gray-200">
                  {sortedProblems.map((problem) => (
                    <tr
                      key={problem.id}
                      onClick={() => handleSelectionChange(problem)}
                      onDoubleClick={() =>
                        navigate(`/database/${problem.problem_id}`)
                      }
                      className={`cursor-pointer transition-colors duration-300 
        ${
          selectedProblems.find(
            (selectedProblem) => selectedProblem.id === problem.problem_id
          )
            ? 'bg-blue-100 hover:bg-blue-200'
            : 'hover:bg-gray-100'
        }`}
                    >
                      <td className="px-6 py-4 whitespace-nowrap">
                        {problem.problem_name}
                      </td>
                      <td className="px-6 py-4 whitespace-nowrap">
                        {problem.num_attempts}
                      </td>
                      <td className="px-6 py-4 whitespace-nowrap">
                        {problem.num_solves}
                      </td>
                      <td className="px-6 py-4 whitespace-nowrap">
                        {problem.first_solve_time}
                      </td>
                    </tr>
                  ))}
                </tbody>
              </table>
            </div>
          )}
        </div>
      </div>

      {/* Buttons Container */}
      <div className="flex justify-between mt-6">
        <button
          className="bg-blue-500 hover:bg-blue-700 text-white font-bold py-2 px-4 rounded"
          onClick={() => navigate(-1)}
        >
          Go Back
        </button>
        <button
          className={`bg-blue-500 text-white font-bold py-2 px-4 rounded`}
          onClick={sendToExportProblems}
        >
          Add Selected Problems to Export List
        </button>
      </div>
    </div>
  );
}

export default ContestDetail;

import { useParams, useNavigate } from 'react-router-dom';
import { useEffect, useState } from 'react';
import { useExportProblems } from '../context/exportProblemsContext';
import Auth from '../utils/auth';
const serverUrl = process.env.REACT_APP_SERVER_BASE_URL;

function ProblemDetail() {
  const navigate = useNavigate();
  const [problemData, setProblemData] = useState(null);
  const [problemInstanceData, setProblemInstanceData] = useState(null);
  const [exportProblems, setExportProblems] = useExportProblems();
  const [sortConfig, setSortConfig] = useState({
    key: null,
    direction: 'none',
  });

  let { id } = useParams();
  // Function to handle sorting logic:
  // - Determines the direction of sorting based on the current state and the column clicked.
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
    fetch(`${serverUrl}/api/problem/${id}`, options)
      .then((response) => response.json())
      .then((data) => {
        setProblemData(data.data);
      });

    fetch(`${serverUrl}/api/probleminstance/problem/${id}`, options)
      .then((response) => response.json())
      .then((data) => {
        const promises = data.data.map((problemInstance) =>
          fetch(
            `${serverUrl}/api/contest/${problemInstance.contest_id}`,
            options
          )
            .then((response) => response.json())
            .then((contestData) => ({
              ...problemInstance,
              contest_name: contestData.data.name,
            }))
        );
        Promise.all(promises).then(setProblemInstanceData);
      });
  }, [id]);
  // Sorting logic for problem instances:
  // - Sorts the problem instances based on the selected column and direction.
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

  // let sortedProblems = fetch(`http://${serverUrl}/api/problem/list?page=1&sort=${sortConfig.key}&order=asc`)
  // .then((response) => response.json())
  // .then((data) => {
  //   return data.data;
  // });

  return (
    <div
      style={{ minHeight: 'calc(100vh - 80px)' }}
      className="h-full min-h-screen flex flex-col max-w-3xl mx-auto bg-white p-6 rounded-lg shadow-md"
    >
      {/* Problem Details */}
      {/* Problem name, ID, author, and total solves are displayed here. */}
      <div className="flex-grow overflow-auto">
        <h1 className="text-2xl font-semibold mb-3">
          Problem Name:{' '}
          <span className="font-normal">{problemData && problemData.name}</span>
        </h1>

        <div className="mb-4">
          <h2 className="text-xl font-medium">Problem ID:</h2>
          <p className="mt-2 text-gray-600">{problemData && problemData.id}</p>
        </div>

        <div className="mb-4">
          <h2 className="text-xl font-medium">Author:</h2>
          <p className="mt-2 text-gray-600">
            {problemData && problemData.author}
          </p>
        </div>
        <div>
          <h2 className="text-xl font-medium">Contests:</h2>
          {problemInstanceData && (
            <table className="min-w-full divide-y divide-gray-200">
              <thead className="bg-gray-50">
                <tr>
                  <th className="px-6 py-3 text-left text-xs font-medium text-gray-500 uppercase tracking-wider">
                    Contest Name
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
                {sortedProblems.map((contest) => (
                  <tr key={contest.id}>
                    <td className="px-6 py-4 whitespace-nowrap">
                      {contest.contest_name}
                    </td>
                    <td className="px-6 py-4 whitespace-nowrap">
                      {contest.num_attempts}
                    </td>
                    <td className="px-6 py-4 whitespace-nowrap">
                      {contest.num_solves}
                    </td>
                    <td className="px-6 py-4 whitespace-nowrap">
                      {contest.first_solve_time}
                    </td>
                  </tr>
                ))}
              </tbody>
            </table>
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
          className={`bg-blue-500 text-white font-bold py-2 px-4 rounded ${
            exportProblems.some(
              (problem) => problemData && problem.id === problemData.id
            )
              ? 'opacity-50 cursor-not-allowed'
              : 'hover:bg-blue-700'
          }`}
          onClick={() => {
            if (
              !exportProblems.some(
                (problem) => problemData && problem.id === problemData.id
              )
            ) {
              setExportProblems((prevProblems) => [
                ...prevProblems,
                problemData,
              ]);
            }
          }}
          disabled={exportProblems.some(
            (problem) => problemData && problem.id === problemData.id
          )}
        >
          {exportProblems.some(
            (problem) => problemData && problem.id === problemData.id
          )
            ? 'Added to Export List'
            : 'Add to Export List'}
        </button>
      </div>
    </div>
  );
}

export default ProblemDetail;

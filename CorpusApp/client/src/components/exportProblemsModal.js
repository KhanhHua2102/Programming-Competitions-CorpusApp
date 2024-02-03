import React, { useState, useEffect } from 'react';
import { useExportProblems } from '../context/exportProblemsContext';
import { useNavigate } from 'react-router-dom';
import Confetti from 'react-dom-confetti';
import Auth from '../utils/auth';

const baseServerUrl = process.env.REACT_APP_SERVER_BASE_URL;

const ExportProblemsModal = ({ isOpen }) => {
  const navigate = useNavigate();
  const modalWidth = '400px'; // Adjust this width as needed
  const [exportProblems, setExportProblems] = useExportProblems();
  const [username, setUsername] = useState('');
  const [password, setPassword] = useState('');
  const [serverUrl, setServerUrl] = useState('');
  const [contestId, setContestId] = useState('');
  const [isLoading, setIsLoading] = useState(false);
  const [isExportSuccessful, setIsExportSuccessful] = useState(false);

  const handleDelete = (id) => {
    const updatedProblems = exportProblems.filter((item) => item.id !== id);
    setExportProblems(updatedProblems);
  };

  const downloadSelected = async () => {
    const exportProblemsId = exportProblems.map((problem) => problem.id);
    if (exportProblemsId.length === 0) {
      alert('No IDs to export');
      return;
    }

    const queryString = exportProblemsId.join(',');
    const token = Auth.getToken();
    const options = {
      headers: {
        Authorization: `Bearer ${token}`,
      },
    };

    const response = await fetch(
      `${baseServerUrl}/api/problem/download?ids=${queryString}`,
      options
    );

    // Check if the server responded with an error
    if (!response.ok) {
      alert('Failed to download files');
      return;
    }

    // Create a blob from the response stream and create a link to download it
    const blob = await response.blob();
    const url = window.URL.createObjectURL(blob);
    const a = document.createElement('a');
    a.href = url;
    a.download = 'files.zip'; // Name for the downloaded file
    document.body.appendChild(a);
    a.click();
    a.remove();
  };

  const exportToDOMjudge = async () => {
    const problemIds = exportProblems.map((problem) => problem.id);
    setIsLoading(true);
    const token = Auth.getToken();

    const response = await fetch(`${baseServerUrl}/api/problem/export`, {
      method: 'POST',
      headers: {
        'Content-Type': 'application/json',
        Authorization: `Bearer ${token}`,
      },
      body: JSON.stringify({
        username,
        password,
        serverUrl,
        contestId,
        problemIds,
      }),
    });

    if (!response.ok) {
      alert(
        'Failed to export problems. Double check your base url and credentials.'
      );
      setIsLoading(false);
      return;
    }
    if (response.ok) {
      setIsExportSuccessful(true);
    }

    alert('Problems successfully uploaded!');
    setIsLoading(false);
  };

  useEffect(() => {
    if (isExportSuccessful) {
      setTimeout(() => {
        setIsExportSuccessful(false);
      }, 2000); // Reset after 5 seconds
    }
  }, [isExportSuccessful]);
  return (
    <div
      style={{ '--modal-width': modalWidth }}
      className={`flex-none transition-width duration-300 ${
        isOpen ? 'w-[var(--modal-width)]' : 'w-0'
      } h-full bg-gray-200  shadow-lg overflow-hidden`}
    >
      <div className="w-full max-w-md mx-auto shadow-md overflow-hidden rounded-lg flex flex-col h-full">
        <div className="p-4 font-bold  bg-gray-300 ">
          Selected Problems for Export
        </div>
        <div className="overflow-auto  bg-gray-200 ">
          {exportProblems.length === 0 ? (
            <div className="text-center py-4">
              <span className="text-gray-700 font-medium">
                No problems selected
              </span>
            </div>
          ) : (
            <table className="min-w-full divide-y divide-gray-200">
              <thead className="bg-gray-50 sticky top-0">
                <tr>
                  <th
                    scope="col"
                    className="px-6 py-3 text-left text-xs font-medium text-gray-500 uppercase tracking-wider"
                  ></th>
                  <th
                    scope="col"
                    className="px-6 py-3 text-left text-xs font-medium text-gray-500 uppercase tracking-wider"
                  >
                    ID
                  </th>
                  <th
                    scope="col"
                    className="px-6 py-3 text-left text-xs font-medium text-gray-500 uppercase tracking-wider"
                  >
                    Name
                  </th>
                </tr>
              </thead>
              <tbody className="bg-white divide-y divide-gray-200">
                {exportProblems.map((item) => (
                  <tr
                    key={item.id}
                    onDoubleClick={() => navigate(`/database/${item.id}`)}
                    className="hover:bg-gray-200 transition-colors duration-200 cursor-pointer"
                  >
                    <td className="px-6 py-4 whitespace-nowrap text-right text-sm font-medium">
                      <button
                        onClick={(e) => {
                          e.stopPropagation();
                          handleDelete(item.id);
                        }}
                        className="text-red-600 hover:text-red-900"
                      >
                        X
                      </button>
                    </td>
                    <td className="px-6 py-4 whitespace-nowrap">
                      <div className="text-sm text-gray-900">{item.id}</div>
                    </td>
                    <td className="px-6 py-4 whitespace-nowrap">
                      <div className="text-sm text-gray-900">{item.name}</div>
                    </td>
                  </tr>
                ))}
              </tbody>
            </table>
          )}
        </div>
        {exportProblems.length > 0 && (
          <div>
            <div className="p-4">
              <label
                className="block text-gray-700 text-sm font-bold mb-2"
                for="server-url"
              >
                DOMJudge base server URL
              </label>
              <input
                type="text"
                id="server-url"
                className="mb-4 w-full px-3 py-2 text-sm leading-tight text-gray-700 border rounded shadow appearance-none focus:outline-none focus:shadow-outline"
                placeholder="E.g. http://123.45.678.90:12345"
                value={serverUrl}
                onChange={(e) => setServerUrl(e.target.value)}
              />
              <label
                className="block text-gray-700 text-sm font-bold mb-2"
                for="username"
              >
                Username
              </label>
              <input
                type="text"
                id="username"
                className="mb-4 w-full px-3 py-2 text-sm leading-tight text-gray-700 border rounded shadow appearance-none focus:outline-none focus:shadow-outline"
                placeholder="E.g. cits3200students"
                value={username}
                onChange={(e) => setUsername(e.target.value)}
              />
              <label
                className="block text-gray-700 text-sm font-bold mb-2"
                for="password"
              >
                Password
              </label>
              <input
                type="password"
                id="password"
                className="mb-4 w-full px-3 py-2 text-sm leading-tight text-gray-700 border rounded shadow appearance-none focus:outline-none focus:shadow-outline"
                placeholder="E.g. cits3200students"
                value={password}
                onChange={(e) => setPassword(e.target.value)}
              />
              <label
                className="block text-gray-700 text-sm font-bold mb-2"
                for="contest-id"
              >
                Contest ID (optional)
              </label>
              <input
                type="text"
                id="contest-id"
                className="mb-4 w-full px-3 py-2 text-sm leading-tight text-gray-700 border rounded shadow appearance-none focus:outline-none focus:shadow-outline"
                placeholder="Enter contest ID"
                value={contestId}
                onChange={(e) => setContestId(e.target.value)}
              />
            </div>
            <div className="flex justify-between p-4 border-t border-gray-200">
              <button
                onClick={downloadSelected}
                className="bg-blue-500 hover:bg-blue-700 text-white font-bold py-2 px-4 rounded"
              >
                Download Zips
              </button>
              <button
                onClick={exportToDOMjudge}
                className={`bg-green-500 hover:bg-green-700 text-white font-bold py-2 px-4 rounded ${
                  (!serverUrl || !username || !password || isLoading) &&
                  'opacity-50 cursor-not-allowed'
                }`}
                disabled={!serverUrl || !username || !password || isLoading}
              >
                {isLoading ? 'Uploading...' : 'Export to DOMjudge'}
                <Confetti active={isExportSuccessful} />
              </button>
            </div>
          </div>
        )}
      </div>
    </div>
  );
};

export default ExportProblemsModal;

import { useNavigate } from 'react-router-dom';
import { useState, useContext, useEffect } from 'react';
import DataContext from '../context/DataContext';

function formatFieldToHeader(key) {
  // Use regular expressions to split the key by camelCase or underscores
  const words = key.split(/(?=[A-Z])|_/);

  // Capitalize the first letter of each word and join them with spaces
  const formattedLabel = words
    .map((word) => {
      // Capitalize the first letter and convert the rest to lowercase
      return word.charAt(0).toUpperCase() + word.slice(1).toLowerCase();
    })
    .join(' ');

  return formattedLabel;
}

const ProblemList = ({
  data,
  onRowClick,
  selectedRows,
  onDelete,
  totalSolves,
  userRole, // Add userRole prop
}) => {
  const [showConfirm, setShowConfirm] = useState(false);
  const [itemToDelete, setItemToDelete] = useState(null);
  const context = useContext(DataContext);
  const [sortField, setSortField] = useState('numSolves'); // default sorting field
  const [sortOrder, setSortOrder] = useState('asc'); // default sorting order
  const [sortedData, setSortedData] = useState(data); // state to hold sorted data

  const navigate = useNavigate();

  const headers = [
    'problem_id',
    'name',
    'num_solves',
    'num_attempts',
    'completion_rate',
  ];

  // Function to handle delete button click
  const handleDeleteClick = (item) => {
    setItemToDelete(item);
    setShowConfirm(true);
  };

  // Function to handle confirm delete
  const handleConfirmDelete = () => {
    onDelete(itemToDelete.problem_id);
    setShowConfirm(false);
  }; // Add closing brace here

  useEffect(() => {
    const dataWithCompletionRate = data.map((item) => ({
      ...item,
      completion_rate: item.num_solves / item.num_attempts,
    }));

    const sorted = [...dataWithCompletionRate].sort((a, b) => {
      if (a[sortField] && b[sortField]) {
        if (typeof a[sortField] === 'number') {
          return sortOrder === 'asc'
            ? a[sortField] - b[sortField]
            : b[sortField] - a[sortField];
        } else {
          return sortOrder === 'asc'
            ? a[sortField].localeCompare(b[sortField])
            : b[sortField].localeCompare(a[sortField]);
        }
      }
    });

    setSortedData(sorted);
  }, [data, sortField, sortOrder]);

  const handleHeaderClick = (field) => {
    setSortField(field);
    setSortOrder((prev) => (prev === 'asc' ? 'desc' : 'asc'));
  };

  return (
    <div className="max-w-7xl mx-auto bg-white p-6 rounded-lg shadow-md overflow-x-auto">
      {showConfirm && (
        <div className="absolute inset-0 flex items-center justify-center bg-black bg-opacity-50">
          <div className="bg-white p-4 rounded-md flex flex-col">
            <div>
              <p>
                Are you sure you want to delete{' '}
                <span className="font-bold">{itemToDelete.name}</span>?
              </p>
              <p>
                This action is <span className="font-bold">not reversible</span>
                , and will also delete associated information from the archive.
              </p>
            </div>
            <div className="mt-4 flex justify-between">
              <button
                onClick={() => setShowConfirm(false)}
                className="bg-gray-500 hover:bg-gray-700 text-white font-bold py-1 px-2 rounded"
              >
                No, take me back
              </button>
              <button
                onClick={handleConfirmDelete}
                className="bg-red-500 hover:bg-red-700 text-white font-bold py-1 px-2 rounded"
              >
                Yes, I'm sure
              </button>
            </div>
          </div>
        </div>
      )}
      <table className="min-w-full leading-normal">
        <thead>
          <tr>
            <th
              className="px-5 py-3 border-b-2 border-gray-200 text-left text-xs font-semibold text-gray-600 uppercase tracking-wider "
              key={'delete'}
            ></th>
            {headers.map((header) => (
              <th
                className="px-5 py-3 border-b-2 border-gray-200 text-left text-xs font-semibold text-gray-600 uppercase tracking-wider cursor-pointer"
                key={header}
                onClick={() => handleHeaderClick(header)}
              >
                {header}
              </th>
            ))}
          </tr>
        </thead>
        {/* Table body: */}
        <tbody>
          {/* If there's data, map through each item and display a row: */}
          {sortedData && sortedData.length > 0 ? (
            sortedData.map((item, index) => (
              <tr
                key={item.id || index}
                onClick={(e) => {
                  if (e.target.tagName !== 'BUTTON') {
                    onRowClick(item);
                  }
                }}
                onDoubleClick={(e) => {
                  if (e.target.tagName !== 'BUTTON') {
                    navigate(`/database/${item.problem_id}`);
                  }
                }}
                className={`cursor-pointer transition-colors duration-300 
            ${
              selectedRows.some((problem) => problem.id === item.id)
                ? 'bg-blue-100 hover:bg-blue-200'
                : 'hover:bg-gray-100'
            }`}
              >
                <td>
                  {(userRole === 'admin' || userRole === 'superuser') && (
                    <button
                      onClick={() => handleDeleteClick(item)}
                      className="bg-red-500 hover:bg-red-700 text-white font-bold py-1 px-2 rounded"
                    >
                      Delete
                    </button>
                  )}
                </td>
                {headers.map((header) => (
                  <td key={header}>
                    {header === 'completion_rate'
                      ? (item.num_solves / item.num_attempts).toFixed(2)
                      : item[header]}
                  </td>
                ))}
              </tr>
            ))
          ) : (
            <tr>
              <td colSpan={headers.length + 1} className="text-center">
                No problems found!
              </td>
            </tr>
          )}
        </tbody>
      </table>
    </div>
  );
};

export { ProblemList };

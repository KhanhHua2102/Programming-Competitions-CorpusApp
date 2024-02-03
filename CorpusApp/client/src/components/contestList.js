import { useNavigate } from 'react-router-dom';
import { useState } from 'react';
import Auth from '../utils/auth';

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

const ContestList = ({
  data,
  onRowClick,
  selectedRows,
  onDelete,
  userRole,
}) => {
  // selectedRows currently unused
  const [showConfirm, setShowConfirm] = useState(false);
  const [itemToDelete, setItemToDelete] = useState(null);

  const navigate = useNavigate();

  const headers = data.length > 0 ? Object.keys(data[0]) : [];

  // Function to handle delete button click
  const handleDeleteClick = (item) => {
    setItemToDelete(item);
    setShowConfirm(true);
  };

  // Function to handle confirm delete
  const handleConfirmDelete = () => {
    onDelete(itemToDelete.id);
    setShowConfirm(false);
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
            {(userRole === 'superuser' || userRole === 'admin') && (
              <th
                className="px-5 py-3 border-b-2 border-gray-200 text-left text-xs font-semibold text-gray-600 uppercase tracking-wider "
                key={'delete'}
              ></th>
            )}
            {headers.map((header) => (
              <th
                className="px-5 py-3 border-b-2 border-gray-200 text-left text-xs font-semibold text-gray-600 uppercase tracking-wider "
                key={header}
              >
                {formatFieldToHeader(header)}
              </th>
            ))}
          </tr>
        </thead>
        <tbody>
          {data && data.length > 0 ? (
            data.map((item, index) => (
              <tr
                key={item.id || index}
                onClick={(e) => {
                  if (e.target.tagName !== 'BUTTON') {
                    onRowClick(item);
                  }
                }}
                onDoubleClick={(e) => {
                  if (e.target.tagName !== 'BUTTON') {
                    navigate(`/contest/${item.id}`);
                  }
                }}
                className={`cursor-pointer transition-colors duration-300 
        ${
          selectedRows.some((problem) => problem.id === item.id)
            ? 'hover:bg-gray-100'
            : 'hover:bg-gray-100'
        }`}
              >
                {(userRole === 'superuser' || userRole === 'admin') && (
                  <td>
                    <button
                      onClick={() => handleDeleteClick(item)}
                      className="bg-red-500 hover:bg-red-700 text-white font-bold py-1 px-2 rounded"
                    >
                      Delete
                    </button>
                  </td>
                )}
                {headers.map((header) => (
                  <td key={header}>{item[header]}</td>
                ))}
              </tr>
            ))
          ) : (
            <tr>
              <td colSpan={headers.length} className="text-center">
                No contests found!
              </td>
            </tr>
          )}
        </tbody>
      </table>
    </div>
  );
};

export { ContestList };

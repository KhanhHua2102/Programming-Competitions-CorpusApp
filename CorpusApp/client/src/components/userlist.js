import { useNavigate } from 'react-router-dom';
import { useState } from 'react';
import Auth from '../utils/auth';

const serverUrl = process.env.REACT_APP_SERVER_BASE_URL;

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

const UserList = ({ data, onRowClick, selectedRows, onDelete }) => {
  // selectedRows currently unused
  const [showConfirm, setShowConfirm] = useState(false);
  const [showEditUser, setEditUser] = useState(false);
  const [itemToDelete, setItemToDelete] = useState(null);
  const [itemToUpdate, setItemToUpdate] = useState(null);
  const [password, setPassword] = useState('');
  const [selectedRole, setSelectedRole] = useState('');

  const navigate = useNavigate();

  const headers = data.length > 0 ? Object.keys(data[0]) : [];

  // Function to handle delete button click
  const handleDeleteClick = (item) => {
    setItemToDelete(item.id);
    setShowConfirm(true);
  };

  // Function to handle update button click
  const handleUpdateClick = (item) => {
    setItemToUpdate(item);
    setEditUser(true);
  };

  // Function to handle update
  const handleUpdateConfirm = (item) => {
    setItemToUpdate(item);
    // logic for handling change confirms
    const token = Auth.getToken();

    let hash_password = null;
    if (password !== '') {
      hash_password = password;
    }
    fetch(`${serverUrl}/api/user/${itemToUpdate.id}`, {
      method: 'PUT',
      headers: {
        'Content-Type': 'application/json',
        token: token,
      },
      body: JSON.stringify({
        username: itemToUpdate.username,
        hash_password: hash_password,
        id: itemToUpdate.id,
        role: selectedRole || itemToUpdate.role,
      }),
    });

    setEditUser(false);
  };

  // Function to handle confirm delete
  const handleConfirmDelete = () => {
    onDelete(itemToDelete);
    setShowConfirm(false);
  };

  return (
    <div className="max-w-7xl mx-auto bg-white p-6 rounded-lg shadow-md overflow-x-auto">
      {showConfirm ? (
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
      ) : null}
      {showEditUser ? (
        <div className="absolute inset-0 flex items-center justify-center bg-black bg-opacity-50">
          <div className="bg-white p-4 rounded-md flex flex-col">
            <div className="flex flex-col">
              <h1 className="font-bold w-full text-center text-3xl">
                Editing User: {itemToUpdate.username}
              </h1>
              <h2 className="w-full text-center text-1xl">
                Current Role: {itemToUpdate.role}
              </h2>
              <div className="flex flex-row p-5 gap-10">
                <div className="flex w-1/2 flex-wrap flex-col justify-between gap-5">
                  <h3 className="w-full font-bold text-center">
                    Edit User Role:
                  </h3>
                  <select
                    className="indent-2 w-full outline outline-2 outline-offset-0 hover:outline-4 rounded-md"
                    value={selectedRole}
                    onChange={(e) => setSelectedRole(e.target.value)}
                  >
                    <option>Select a Role</option>
                    <option value="superuser">Super Admin</option>
                    <option value="admin">Admin</option>
                    <option value="readonly">Read Only</option>
                  </select>
                </div>
                <div className="flex w-1/2 flex-wrap flex-col justify-between">
                  <h3 className="font-bold w-full text-center">
                    Edit User Password:
                  </h3>
                  <input
                    className="w-full outline outline-2 outline-offset-0 hover:outline-4 rounded-md indent-2"
                    placeholder="**********"
                    type="password"
                    value={password}
                    onChange={(e) => setPassword(e.target.value)}
                  ></input>
                </div>
              </div>
            </div>
            <div className="mt-4 flex justify-between">
              <button
                onClick={() => {
                  setEditUser(false);
                  setSelectedRole(null);
                  setPassword('');
                }}
                className="bg-gray-500 hover:bg-gray-700 text-white font-bold py-1 px-2 rounded"
              >
                Take me back
              </button>
              <button
                onClick={handleUpdateConfirm}
                className="bg-red-500 hover:bg-red-700 text-white font-bold py-1 px-2 rounded"
              >
                Confirm Changes
              </button>
            </div>
          </div>
        </div>
      ) : null}
      <table className="min-w-full leading-normal">
        <thead>
          <tr>
            <th
              className="px-5 py-3 border-b-2 border-gray-200 text-left text-xs font-semibold text-gray-600 uppercase tracking-wider "
              key={'delete'}
            ></th>
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
                  /*
                  if (e.target.tagName !== 'BUTTON') {
                    onRowClick(item);
                  }
                  */
                }}
                onDoubleClick={(e) => {
                  if (e.target.tagName !== 'BUTTON') {
                    navigate(`/contest/${item.id}`);
                  }
                }}
                className={`cursor-pointer transition-colors duration-300`}
              >
                {item.role !== 'superuser' ? (
                  <td className="h-10">
                    <button
                      onClick={() => handleDeleteClick(item)}
                      className="bg-red-500 ml-12 hover:bg-red-700 text-white font-bold py-1 px-2 rounded"
                    >
                      Delete
                    </button>
                    <button
                      onClick={() => handleUpdateClick(item)}
                      className="bg-blue-500 ml-5 hover:bg-blue-700 text-white font-bold py-1 px-2 rounded"
                    >
                      Update
                    </button>
                  </td>
                ) : null}
                {item.role === 'superuser' ? (
                  <td className="font-bold h-10">
                    You cannot edit other SuperAdmin's
                  </td>
                ) : null}
                {headers.map((header) => (
                  <td key={header}>{item[header]}</td>
                ))}
              </tr>
            ))
          ) : (
            <tr>
              <td colSpan={headers.length} className="text-center">
                No Users found!
              </td>
            </tr>
          )}
        </tbody>
      </table>
    </div>
  );
};

export { UserList };

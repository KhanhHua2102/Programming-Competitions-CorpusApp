import Auth from '../utils/auth';

function Logout() {
  localStorage.clear();
  Auth.logout();
}

export default Logout;

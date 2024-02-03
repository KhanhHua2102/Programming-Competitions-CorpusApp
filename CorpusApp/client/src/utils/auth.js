import decode from 'jwt-decode';

class AuthService {
  getUserRole() {
    const token = this.getToken();

    if (!token) return null;
    try {
      const decoded = decode(token);

      return decoded.role;
    } catch (err) {
      console.error('Invalid token', err);
      return null;
    }
  }

  getProfile() {
    return localStorage.getItem('userRole');
  }

  loggedIn() {
    const token = this.getToken();
    // If there is a token and it's not expired, return `true`
    return token && !this.isTokenExpired(token) ? true : false;
  }

  isTokenExpired() {
    const token = this.getToken();
    // Decode the token to get its expiration time that was set by the server
    const decoded = decode(token);
    // If the expiration time is less than the current time (in seconds), the token is expired and we return `true`
    if (decoded.exp < Date.now() / 1000) {
      localStorage.clear();
      return true;
    }
    // If token hasn't passed its expiration time, return `false`
    return false;
  }

  getToken() {
    return localStorage.getItem('userToken');
  }

  getUsername() {
    return localStorage.getItem('userName');
  }

  login(idToken) {
    localStorage.setItem('userToken', idToken);
    window.location.assign('/');
  }

  logout() {
    localStorage.removeItem('userToken');
    window.location.assign('/');
  }
}
const Auth = new AuthService();

export default Auth;

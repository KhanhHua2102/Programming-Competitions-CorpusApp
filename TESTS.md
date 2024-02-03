# Corpus App: Tests

This document contains documentation for running existing tests on `Corpus App`.

## Client

This guide will help you run the Selenium IDE test for our application using the `frontend_test.SIDE` file.

### Prerequisites

Ensure you have either [Google Chrome](https://www.google.com/chrome/) or [Mozilla Firefox](https://www.mozilla.org/firefox/download/) installed, as Selenium IDE is _only_ available for these browsers.

Before running tests, ensure that:
- A local version of the client is running. You can do this with `npm run start` from the base `/CorpusApp` directory.
- You are logged in as "superuser".
- The only problems in the database are uploaded from the `CorpusApp/server/__tests__/contest/crud/domjudge_problems.zip` file.

### Step 1: Install Selenium IDE

1. **For Google Chrome Users:**
   - Visit the [Chrome Web Store](https://chrome.google.com/webstore/category/extensions) and search for "Selenium IDE".
   - Click on "Add to Chrome" next to Selenium IDE and confirm the installation.

2. **For Mozilla Firefox Users:**
   - Visit the [Firefox Add-ons store](https://addons.mozilla.org/en-US/firefox/) and search for "Selenium IDE".
   - Click on "Add to Firefox" next to Selenium IDE and confirm the installation.

### Step 2: Download the Test File

Download the `frontend_test.SIDE` file from the repository or project documentation. This file, (`client_tests.side`), can be found at the root of the project. 

### Step 3: Run the Selenium Test

1. **Launch Selenium IDE:**
   - Click on the Selenium IDE icon in your browser’s toolbar.

2. **Open the Test File:**
   - In Selenium IDE, click on "Open an existing project".
   - Locate and select the `frontend_test.SIDE` file.

3. **Run the Test:**
   - To run a single test, select the test in the IDE, and click on the "Run current test" button.
   - To run all tests in the suite, click on the "Run all tests" button.

### Step 4: View Test Results

- Test results will be displayed in the Selenium IDE, with passed steps in green and failed steps in red.
- Use the "Log" tab to investigate any issues encountered during the test run.

#### Step 5: Debugging (if needed)

- If there are failed steps, you may need to debug them.
- Edit test scripts, update assertions, or modify steps to resolve issues.

#### Note:

- If running tests programmatically or in a CI/CD pipeline is needed, you may export tests to different programming languages using the "Export" feature in Selenium IDE, and then use Selenium WebDriver to execute them. Check the [Selenium documentation](https://www.selenium.dev/documentation/en/selenium_ide/) for further information on running tests with Selenium WebDriver.

### Additional Resources

- [Selenium IDE Documentation](https://www.selenium.dev/selenium-ide/docs/en/introduction/getting-started)
- [Selenium WebDriver Documentation](https://www.selenium.dev/documentation/en/webdriver/)


## Server

To run the tests, simply run `cd CorpusApp/server`, and run `npm run test` to run the tests.

**NOTE** due to a bug in the DOMJudge API, the export problem test sometimes incorrectly fails. 

### Setup Instructions

Before all tests, an Express application is created and the contest routes are added to it. A SQLite database is also set up with tables for contests, problems, and problem instances. Some initial data is seeded into these tables.

Before each test, a token is generated for authentication and a new transaction is started in the database.

After each test, the transaction is rolled back to ensure that each test starts with a clean state.

After all tests, any directories created for testing are removed.

#### Test Cases

1. **Contest GET Requests**

These tests ensure that the GET routes for retrieving all contests and a single contest by ID are working correctly.

- Should get all contests: This test sends a GET request to the /api/contest route and checks that the response status code is 200 and the response body contains the correct data.

- Should get a single contest: This test sends a GET request to the /api/contest/1 route and checks that the response status code is 200 and the response body contains the correct data.

2. **Parsing config.yaml**

These tests ensure that the PUT route for creating or updating a contest correctly handles the uploaded YAML configuration file.

- Should return 400 if no file is uploaded: This test sends a PUT request to the /api/contest route without attaching a file and checks that the response status code is 400 and the response text is "No file uploaded".

- Should return 400 if no .yaml or .yml file in zipped directory: This test sends a PUT request to the /api/contest route with a zipped directory that does not contain a .yaml or .yml file and checks that the response status code is 400 and the response text is "Error: no .yaml file in zipped directory. Did you forget to include it?".

- Should return 400 if YAML content is empty: This test sends a PUT request to the /api/contest route with a zipped directory that contains an empty .yaml file and checks that the response status code is 400 and the response text is "Error: the YAML is not correctly formatted. Please refer to the documentation for correct formatting.".

- Should return 400 if YAML content: contest is missing key fields (name, date): This test sends a PUT request to the /api/contest route with a zipped directory that contains a .yaml file missing key fields in the contest section and checks that the response status code is 400 and the response text is "Error: the YAML is not correctly formatted. Please refer to the documentation for correct formatting.".

- Should return 400 if YAML content problem is missing key fields (name, author): This test sends a PUT request to the /api/contest route with a zipped directory that contains a .yaml file missing key fields in the problem section and checks that the response status code is 400 and the response text is "Error: the YAML is not correctly formatted. Please refer to the documentation for correct formatting.".

3. **Corpus CRUD operations**

These tests ensure that the PUT and DELETE routes for creating, updating, and deleting contests are working correctly.

- Should return 400 if no file is uploaded: This test sends a PUT request to the /api/contest route without attaching a file and checks that the response status code is 400 and the response text is "No file uploaded".

- Should return 200 if a full contest is added successfully: This test sends a PUT request to the /api/contest route with a zipped directory that contains a valid .yaml file and checks that the response status code is 200 and the response text is "All items processed successfully". It then sends a GET request to the /api/contest route and checks that the response body contains the correct data.

- Should return 200 if a full contest is added successfully, and then the metadata is updated with only a config file: This test is similar to the previous one, but after adding the contest, it sends another PUT request to the /api/contest route with a zipped directory that contains only a .yaml file and checks that the response status code is 200 and the response text is "All items processed successfully". It then sends a GET request to the /api/contest route and checks that the response body contains the correct data.

- Should return 200 if a full contest is deleted successfully: This test sends a DELETE request to the /api/contest/1 route and checks that the response status code is 200. It then sends a GET request to the /api/contest route and checks that the response body contains the correct data.

4. **Testing DOMJudge Export Functionality**

- This test case performs the following steps:
    - Sends a PUT request to the /api/contest endpoint, attaching a file named domjudge_problems.zip. This simulates the action of creating a new contest and uploading a set of problems.
    - Sends a GET request to the /api/problem endpoint to retrieve all problems.
    - Sends a POST request to the /api/problem/export endpoint, sending the IDs of the problems retrieved in step 2. This simulates the action of exporting the problems to a DOMJudge server.

5. **Testing User Authorization**

- Should return 200 if admin-only permissions can be accessed by an admin. This test sends a PUT request to the /api/contest route with a zipped directory that contains a valid .yaml file and checks that the response status code is 200 and the response text is "All items processed successfully".
- Should return 403 if admin-only permissions can be accessed by a user without admin role. This test sends a PUT request to the /api/contest route with a zipped directory that contains a valid .yaml file, but with an unauthenticated token, and checks that the response status code is 403.

6. **Problem GET Requests**

These tests ensure that the GET routes for retrieving all problems and a single problem by ID are working correctly.

- Should get all problem: This test sends a GET request to the /api/problem route and checks that the response status code is 200 and the response body contains the correct data.

- Should get a single problem: This test sends a GET request to the /api/problem/1 route and checks that the response status code is 200 and the response body contains the correct data.

7. **ProblemInstance GET Requests**

These tests ensure that the GET routes for retrieving all probleminstances and a single probleminstance by ID are working correctly.

- Should get all probleminstance: This test sends a GET request to the /api/probleminstance route and checks that the response status code is 200 and the response body contains the correct data.

- Should get a single probleminstance: This test sends a GET request to the /api/probleminstance/1 route and checks that the response status code is 200 and the response body contains the correct data.

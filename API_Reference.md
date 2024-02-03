# CorpusApp

## Contents

- [Base URL](#base-url)
- [Error Codes](#error-codes)
- [Resources](#resources)

  - [GET /contest/](#get-contest)
  - [GET /contest/:id](#get-contestid)
  - [GET /problem/](#get-problem)
  - [GET /problem/:id](#get-problemid)
  - [GET /problem/list](#get-problemlist)
  - [GET /probleminstance/](#get-probleminstance)
  - [GET /probleminstance/:id](#get-probleminstanceid)
  - [GET /probleminstance/filter](#get-probleminstancefilter)
  - [GET /probleminstance/contest/:id](#get-probleminstancecontestid)
  - [GET /probleminstance/problem/:id](#get-probleminstanceproblemid)
  - [GET /search/](#get-search)
  - [PUT /contest/](#put-contest)
  - [DELETE /contest/:id](#delete-contestid)
  - [DELETE /problem/:id](#delete-problemid)
  - [DELETE /probleminstance/:id](#delete-probleminstanceid)
  - [POST /problem/export](#post-problemexport)
  - [GET /problem/download](#get-problemdownload)

## Base URL

All API requests are made to:

```
http://localhost:3000/api/
```

by default, OR

```
http://{url}/api/
```

This will change depending on where the server is hosted.

## Error Codes

| Status Code | Description                                                                                                     |
| ----------- | --------------------------------------------------------------------------------------------------------------- |
| 200         | OK - The request was successful.                                                                                |
| 400         | Bad Request - The request could not be understood or was missing required parameters.                           |
| 401         | Unauthorized - Authentication failed or was not provided.                                                       |
| 403         | Forbidden - Authentication succeeded but the authenticated user does not have access to the requested resource. |
| 404         | Not Found - The requested resource could not be found.                                                          |
| 500         | Internal Server Error - An error occurred on the server.                                                        |

## Authentication

To authenticate with the API, you need to send a POST request to the /login endpoint. This request should include your username and password in the request body. Here's an example using curl:

```
curl -X POST -H "Content-Type: application/json" -d '{"username":"your_username","password":"your_password"}' http://localhost:3001/login
```

If the login is successful, the server will respond with a JSON Web Token (JWT). Tokens are valid for 2 hours by default. This token is used to authenticate your subsequent API calls. The response will look something like this:

```
{
    "token": "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJzdWIiOiIxMjM0NTY3ODkwIiwibmFtZSI6IkpvaG4gRG9lIiwiaWF0IjoxNTE2MjM5MDIyfQ.SflKxwRJSMeKKF2QT4fwpMeJf36POk6yJV_adQssw5c"
}
```

To use this token, include it in the Authorization header of your subsequent API calls. The header should look like this:

```
Authorization: Bearer eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJzdWIiOiIxMjM0NTY3ODkwIiwibmFtZSI6IkpvaG4gRG9lIiwiaWF0IjoxNTE2MjM5MDIyfQ.SflKxwRJSMeKKF2QT4fwpMeJf36POk6yJV_adQssw5c
```

Please note that the token is sensitive information and should be handled securely.

## Resources

### GET /contest/

#### Endpoint:

`GET /Contest/`

#### Description

Returns all contests.

#### Parameters:

| Name   | Type | Required | Description                                     |
| ------ | ---- | -------- | ----------------------------------------------- |
| `page` | Int  | No       | Int for pagination. Defaults to the first page. |

#### Response:

Returns a `data` object, which is an array of `Contest`s, and a `meta` dictionary, which holds a `page` value (for pagination).

##### Example success response:

```js
{
    "data": [
        {
            "id": 55,
            "name": "My Test Contest!",
            "description": "Guys... everyone go watch the one piece live action...",
            "region": "ANZ",
            "date": "15/09/2023"
        }
    ],
    "meta": {
        "page": 1
    }
}
```

### GET /contest/:id

#### Endpoint:

`GET /Contest/:id`

#### Description

Returns a single contest.

#### Parameters:

| Name | Type | Required | Description              |
| ---- | ---- | -------- | ------------------------ |
| `id` | Int  | Yes      | Unique ID for a contest. |

#### Response:

Returns a `data` object with contest information, or an empty object if nothing was found.

##### Example success response:

```js
{
    "data": {
        "id": 55,
        "name": "My Test Contest!",
        "description": "Guys... everyone go watch the one piece live action...",
        "region": "ANZ",
        "date": "15/09/2023"
    }
}
```

### GET /problem/

#### Endpoint:

`GET /Problem/`

#### Description

Returns all Problems in the database.

#### Parameters:

| Name   | Type | Required | Description                                     |
| ------ | ---- | -------- | ----------------------------------------------- |
| `page` | Int  | No       | Int for pagination. Defaults to the first page. |

#### Response:

Returns a `data` object, which is an array of `Problem`s, and a `meta` dictionary, which holds a `page` value (for pagination).

##### Example success response:

```js
{
    "data": [
        {
            "id": 87,
            "name": "Zoro's Three Swords",
            "author": "Sanjee",
            "text_metadata": null,
            "is_valid": null,
            "optional": null
        },
        {
            "id": 88,
            "name": "Luffy's Dillema",
            "author": "Sanjee",
            "text_metadata": null,
            "is_valid": null,
            "optional": null
        }
    ],
    "meta": {
        "page": 1
    }
}
```

### GET /problem/:id

#### Endpoint:

`GET /Problem/:id`

#### Description

Returns a single problem.

#### Parameters:

| Name | Type | Required | Description              |
| ---- | ---- | -------- | ------------------------ |
| `id` | Int  | Yes      | Unique ID for a problem. |

#### Response:

Returns a `data` object with problem information, or an empty object if nothing was found.

##### Example success response:

```js
{
    "data": {
        "id": 87,
        "name": "Zoro's Three Swords",
        "author": "Sanjee",
        "text_metadata": null,
        "is_valid": null,
        "optional": null
    }
}
```

### GET /problem/list

#### Endpoint:

`GET /problem/list`

#### Description

Returns problems with all their metadata with with support for sorting and pagination.

#### Parameters:

| Name    | Type   | Required | Description                                     |
| ------- | ------ | -------- | ----------------------------------------------- |
| `page`  | Int    | No       | Int for pagination. Defaults to the first page. |
| `sort`  | String | No       | String for sorting. Defaults to `id`.           |
| `order` | String | No       | String for ordering. Defaults to `asc`.         |

#### Response:

Returns a `data` object, which is an array of `Problem`s, and a `meta` dictionary, which holds a `page` value (for pagination).

##### Example success response:

```js
{
    "data": [
        {
            "id": 239,
            "name": "Zoro's Three Swords",
            "author": "Sanjee",
            "text_metadata": null,
            "is_valid": null,
            "optional": null,
            "contest_id": 109,
            "problem_id": 276,
            "first_solve_time": 900,
            "num_solves": 6,
            "num_attempts": 15,
            "completion_rate": 0.4
        },
        {
            "id": 242,
            "name": "Hello World",
            "author": "DOMJudge Example",
            "text_metadata": null,
            "is_valid": null,
            "optional": null,
            "contest_id": 110,
            "problem_id": 279,
            "first_solve_time": 300,
            "num_solves": 2,
            "num_attempts": 5,
            "completion_rate": 0.4
        },
    ],
    "meta": {
        "page": "1"
    }
}
```

### GET /probleminstance/

#### Endpoint:

`GET /probleminstance/`

#### Description

Returns all problemInstances in the database.

#### Parameters:

| Name   | Type | Required | Description                                     |
| ------ | ---- | -------- | ----------------------------------------------- |
| `page` | Int  | No       | Int for pagination. Defaults to the first page. |

#### Response:

Returns a `data` object, which is an array of `problemInstance`s, and a `meta` dictionary, which holds a `page` value (for pagination).

##### Example success response:

```js
{
    "data": [
        {
            "id": 67,
            "contest_id": 55,
            "problem_id": 87,
            "first_solve_time": 300,
            "num_solves": 2,
            "num_attempts": 5,
            "optional": null
        },
        {
            "id": 68,
            "contest_id": 55,
            "problem_id": 88,
            "first_solve_time": 15,
            "num_solves": 3,
            "num_attempts": 17,
            "optional": null
        }
    ],
    "meta": {
        "page": 1
    }
}
```

### GET /probleminstance/:id

#### Endpoint:

`GET /probleminstance/:id`

#### Description

Returns a single problemInstance.

#### Parameters:

| Name | Type | Required | Description              |
| ---- | ---- | -------- | ------------------------ |
| `id` | Int  | Yes      | Unique ID for a problem. |

#### Response:

##### Example success response:

```js
{
    "data": {
        "id": 87,
        "name": "Zoro's Three Swords",
        "author": "Sanjee",
        "text_metadata": null,
        "is_valid": null,
        "optional": null
    }
}
```

### GET /probleminstance/filter

#### Endpoint:

`GET /probleminstance/filter`

#### Description

Returns all problems with their metadata with support for sorting, filtering and pagination.

#### Parameters:

| Name        | Type   | Required | Description                                     |
| ----------- | ------ | -------- | ----------------------------------------------- |
| `page`      | Int    | No       | Int for pagination. Defaults to the first page. |
| `sort`      | String | No       | String for sorting. Defaults to `id`.           |
| `order`     | String | No       | String for ordering. Defaults to `asc`.         |
| `filter`    | String | No       | String for filtering by column.                 |
| `condition` | String | No       | String for filtering by condition.              |

#### Response:

Returns a `data` object, which is an array of `Problem`s, and a `meta` dictionary, which holds a `page` value (for pagination).

##### Example success response:

```js
{
    "data": [
        {
            "id": 242,
            "name": "Hello World",
            "author": "DOMJudge Example",
            "text_metadata": null,
            "is_valid": null,
            "optional": null,
            "contest_id": 110,
            "problem_id": 279,
            "first_solve_time": 300,
            "num_solves": 2,
            "num_attempts": 5
        },
        {
            "id": 243,
            "name": "Float special compare test",
            "author": "DOMJudge Example",
            "text_metadata": null,
            "is_valid": null,
            "optional": null,
            "contest_id": 110,
            "problem_id": 280,
            "first_solve_time": 300,
            "num_solves": 2,
            "num_attempts": 5
        },
    ],
    "meta": {
        "page": "1"
    }
}
```

### GET /probleminstance/contest/:id

#### Endpoint:

`GET /probleminstance/contest/:id`

#### Description

Returns all the problemInstances in a given contest.

#### Parameters:

| Name | Type | Required | Description              |
| ---- | ---- | -------- | ------------------------ |
| `id` | Int  | Yes      | Unique ID for a contest. |

#### Response:

Returns a `data` object, which is an array of `ProblemInstance`s.

##### Example success response:

```js
{
    "data": [
        {
            "id": 67,
            "contest_id": 55,
            "problem_id": 87,
            "first_solve_time": 300,
            "num_solves": 2,
            "num_attempts": 5,
            "optional": null
        },
        {
            "id": 68,
            "contest_id": 55,
            "problem_id": 88,
            "first_solve_time": 15,
            "num_solves": 3,
            "num_attempts": 17,
            "optional": null
        }
    ]
}
```

### GET /probleminstance/problem/:id

#### Endpoint:

`GET /probleminstance/problem/:id`

#### Description

Returns all the problemInstances for a given Problem.

#### Parameters:

| Name | Type | Required | Description              |
| ---- | ---- | -------- | ------------------------ |
| `id` | Int  | Yes      | Unique ID for a problem. |

#### Response:

Returns a `data` object, which is an array of `ProblemInstance`s.

##### Example success response:

```js
{
    "data": [
        {
            "id": 67,
            "contest_id": 55,
            "problem_id": 87,
            "first_solve_time": 300,
            "num_solves": 2,
            "num_attempts": 5,
            "optional": null
        }
    ]
}
```

### GET /search/

#### Endpoint:

`GET /search/`

#### Description

Used for searching for Problems, or Contests by name.

#### Parameters:

| Name   | Type   | Required | Description                                    |
| ------ | ------ | -------- | ---------------------------------------------- |
| `name` | String | Yes      | The name or substring of a problem or contest. |

#### Response:

Returns an object with:

- a `problems` field, containing a `data` object with a list of matching problems
- a `contest` field, containing a `data` object with a list of matching contests

##### Example success response:

```js
{
    "problems": {
        "data": []
    },
    "contests": {
        "data": [
            {
                "id": 55,
                "name": "My Test Contest!",
                "description": "Guys... everyone go watch the one piece live action...",
                "region": "ANZ",
                "date": "15/09/2023"
            }
        ]
    }
}
```

### PUT /contest/

Look at the [config.yaml](README.md#api-reference-and-configyaml-structure) section for details on how to use.

#### Endpoint:

`PUT /Contest/`

#### Description

Allows for the upload of a single .zip or .tar file representing a contest. This file should contain directories (or zips) for each problem, accompanied by a config.yaml that provides the contest and problem metadata.

Can also be used to update a contest directly. Simply upload a zip of a config.yaml with the relevant information and it will automatically add rows in the database (but not upload directories to the bucket).

#### Parameters:

| Name   | Type | Required | Description                                     |
| ------ | ---- | -------- | ----------------------------------------------- |
| `page` | Int  | No       | Int for pagination. Defaults to the first page. |

#### Response:

##### Example success response:

```js
{
    All items processed successfully
}
```

### DELETE /contest/:id

**Must be authenticated as admin or superuser**

#### Endpoint:

`DELETE /contest/:id`

#### Description

Deletes a specific contest from the Corpus.

#### Parameters:

None

#### Response:

Returns a status 200 if successful.

##### Example success response:

```js
{
    Contest deleted successfully
}
```

### DELETE /problem/:id

**Must be authenticated as admin or superuser**

#### Endpoint:

`DELETE /problem/:id`

#### Description

Deletes a specific problem from the Corpus.

#### Parameters:

None

#### Response:

Returns a status 200 if successful.

##### Example success response:

```js
{
    Problem deleted successfully
}
```

### DELETE /probleminstance/:id

**Must be authenticated as admin or superuser**

#### Endpoint:

`DELETE /probleminstance/:id`

#### Description

Deletes a specific probleminstance from the Corpus.

#### Parameters:

None

#### Response:

Returns a status 200 if successful.

##### Example success response:

```js
{
    Probleminstance deleted successfully
}
```

### POST problem/export

#### Endpoint:

`POST /problem/export`

#### Description

Posts a set of problems from the Corpus directly to a DOMJudge instance.

#### Parameters:

| Name       | Type   | Required | Description                                                                    |
| ---------- | ------ | -------- | ------------------------------------------------------------------------------ |
| username   | String | Yes      | The username for the DOMJudge server.                                          |
| password   | String | Yes      | The password for the DOMJudge server.                                          |
| serverUrl  | String | Yes      | The URL of the DOMJudge server.                                                |
| contestId  | String | No       | The ID of the contest on the DOMJudge server.                                  |
| problemIds | Array  | Yes      | An array of problem IDs to be exported from the Corpus to the DOMJudge server. |

#### Response:

Returns a status 200 if successful.
Returns a status 500 if it failed.

##### Example success response:

```js
{
  ('Problems exported successfully');
}
```

### GET /problem/download

#### Endpoint:

GET /problem/download

#### Description

Downloads a zipped directory containing problems, given a list of IDs.

#### Parameters:

| Name | Type   | Required | Description                                             |
| ---- | ------ | -------- | ------------------------------------------------------- |
| ids  | String | Yes      | A comma-separated list of problem IDs to be downloaded. |

#### Response:

Returns a status 200 and a zipped file if successful. Returns a status 400 and an error message if no IDs are provided.

#### Example success response:

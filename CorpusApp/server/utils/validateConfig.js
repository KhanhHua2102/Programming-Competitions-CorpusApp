const Ajv = require("ajv");

function validateYamlFile(data) {
  const ajv = new Ajv();
  const validate = ajv.compile(schema);
  const valid = validate(data);
  //console.log('Validation result:', valid);
  //console.log('Validation errors:', validate.errors);
  return valid;
}

const schema = {
  type: "object",
  allOf: [
    {
      properties: {
        contest: {
          type: "object",
          properties: {
            name: { type: "string" },
            date: { type: "string" },
            description: { type: "string" },
            region: { type: "string" },
          },
          required: ["name", "date"],
        },
      },
    },
    {
      properties: {
        problems: {
          type: "array",
          items: {
            type: "object",
            properties: {
              name: { type: "string" },
              dir_name: { type: "string" },
              author: { type: "string" },
              first_solve_time: { type: "number" },
              num_solves: { type: "number" },
              num_attempts: { type: "number" },
            },
            required: ["name", "author"],
          },
        },
      },
    },
  ],
};

module.exports = validateYamlFile;

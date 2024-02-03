const env = process.env;

const config = {
  listPerPage: env.LIST_PER_PAGE || 100,
};

module.exports = config;

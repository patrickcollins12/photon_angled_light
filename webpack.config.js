const path = require('path')
const webpack = require('webpack'); //to access built-in plugins


module.exports = {
  plugins: [
  ],
  module: {
    rules: [
      { test: /\.css$/, use: 'css-loader' },
    ]
  }

 
}

// module.exports = config;

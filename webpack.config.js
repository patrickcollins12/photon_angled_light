const path = require('path')
const webpack = require('webpack'); //to access built-in plugins
const nodeExternals = require('webpack-node-externals')
const HtmlWebPackPlugin = require("html-webpack-plugin")


module.exports = {
  plugins: [
  ],
  module: {
    rules: [
      { test: /\.css$/, use: 'css-loader' },
      {
        // Transpiles ES6-8 into ES5
        test: /\.js$/,
        exclude: /node_modules/,
        use: {
          loader: "babel-loader"
        }
      },

    ]
  }

 
}

// module.exports = config;

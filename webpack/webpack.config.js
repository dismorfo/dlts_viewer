const path = require('path')
const webpack = require('webpack')
module.exports = {
  entry: {
    app: './src/index.js'
  },
  devtool: 'inline-source-map',  
  output: {
    filename: '[name].bundle.js',
    path: path.resolve(__dirname, 'dist/public'),
    publicPath: '/'
  }
}

'use strict'

const fs = require('fs')
const low = require('lowdb')
const FileSync = require('lowdb/adapters/FileSync')
const express = require('express')
const path = require('path')
const logger = require('morgan')
const bodyParser = require('body-parser')
const app = express()

app.set('views', path.join(__dirname, 'dist', 'views'))
app.set('view engine', 'hbs')
app.use(logger('dev'))
app.use(bodyParser.json())
app.use(bodyParser.urlencoded({ extended: false }))
app.use(express.static(path.join(__dirname, 'dist', 'public')))
app.use(require('express-bulma')('/bulma.css'))

app.get('/', (req, res, next) => {
  const adapter = new FileSync(path.join(__dirname, 'dist', 'db.json'))
  const db = low(adapter)
  res.render('index', {
    title: 'DLTS Viewer',
    examples: db.get('examples').value()
  })
})

app.get('/:type/:identifier/:sequence', (req, res, next) => {
  const adapter = new FileSync(path.join(__dirname, 'dist', 'db.json'))
  const db = low(adapter)
  const type = req.params.type
  const pjax = (req.headers['x-pjax']) ? true : false
  const template = pjax ? type + '-pjax' : type
  let book = db.get('books').find({ identifier: req.params.identifier }).value()
  let sequence = db.get('entities').find({ isPartOf: req.params.identifier, realPageNumber: parseInt(req.params.sequence, 10) }).value()
  console.log(book)
  console.log(sequence)
  if (sequence) {
    sequence.tiles = JSON.stringify(sequence.data)
    sequence.identifier = sequence.isPartOf
    res.render(template, sequence)
  }
  else {
    let err = new Error('Not Found')
        err.status = 404
    next(err)
  }
})

// catch 404 and forward to error handler
app.use((req, res, next) => {
  var err = new Error('Not Found')
  err.status = 404
  next(err)
})

// error handler
app.use((err, req, res, next) =>  {
  res.locals.message = err.message
  res.locals.error = req.app.get('env') === 'development' ? err : {}
  res.status(err.status || 500)
  res.render('error')
})

module.exports = app

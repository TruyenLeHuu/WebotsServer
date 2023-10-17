const express = require('express')
const bodyParser = require('body-parser')
const cors = require('cors')
const app = express()
const path = require("path")
const PORT = process.env.PORT ||  3001
const results = require('./models/results')
const mongoose = require('mongoose');

//const URI = 'mongodb://localhost:27017/testdb'
// const URI = 'mongodb+srv://1111:1234@mernprojectceec.byvhv.mongodb.net/MERN_PROJECTCEEC?retryWrites=true&w=majority' 
const URI = "mongodb+srv://1111:1234@cluster0.lxqs9wg.mongodb.net/?retryWrites=true&w=majority"
var server = require("http").Server(app)
var io = require("socket.io")(server)
const webot = require('./routers/webot')(io)
mongoose
.connect(URI, {useNewUrlParser:true, useUnifiedTopology:true})
.then(()=>{
    console.log("Connect to db")
    server.listen(PORT,()=>{
        console.log(`Server is listening on port ${PORT}`)
    })
}).catch((err) => {
    console.log(err)
})
app.use(bodyParser.json({limit: '50mb'}))
app.use(bodyParser.urlencoded({extended:true, limit:"50mb", parameterLimit: 100000}))
app.use(cors({
  origin: '*',
  methods: ['GET','POST','DELETE','UPDATE','PUT','PATCH'],
}));
app.use('/', webot)
app.use('/',express.static(path.join(__dirname, 'public')));
app.set('view engine','ejs');
app.set('views', path.join(__dirname, 'views'));

const changeResult = results.watch();
changeResult.on('change', async() => {
    let list_results = await results.find().select('name school score time rawTime -_id');
    list_results.sort((a, b) => {
        if (a.score === b.score) {
            return a.rawTime - b.rawTime;
        }
        return b.score - a.score;
    });
    let filteredResult = [];
    let nameSet = new Set();

    for (let i = 0; i < list_results.length; i++) {
        if (!nameSet.has(list_results[i].name)) {
            filteredResult.push(list_results[i]);
            nameSet.add(list_results[i].name);
        }
    }
    io.emit('changeResults', filteredResult);
}); 
const mongoose = require('mongoose')
const Schema  = mongoose.Schema
const resultsSchema = new Schema ({
    name:{
        type:String
    },
    controller:{
        type:String   
    },
    school: {
        type:String
    },
    score: {
        type:Number
    },
    time:{
        type : String
    },
    rawTime:{
        type : Number
    },
}, {timestamps:true})
const resultsModel = mongoose.model('Results', resultsSchema)
module.exports=resultsModel
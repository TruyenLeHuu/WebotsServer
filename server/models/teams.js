const mongoose = require('mongoose')
const Schema  = mongoose.Schema
const teamsSchema = new Schema ({
    id: {
        type:Number
    },
    name:{
        type:String
    },
    controller:{
        type:String
    },
    school: {
        type:String
    }
}, {timestamps:true})
const teamsModel = mongoose.model('Teams', teamsSchema)
module.exports=teamsModel
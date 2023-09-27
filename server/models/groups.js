const mongoose = require('mongoose')
const Schema  = mongoose.Schema
const groupsSchema = new Schema ({
    name: {
        type:String
    },
    index: {
        type:Number
    },
    idTeamA:{
        type:Number
    },
    idTeamB:{
        type:Number
    }
}, {timestamps:true})
const groupsModel = mongoose.model('Groups', groupsSchema)
module.exports=groupsModel
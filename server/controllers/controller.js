const groups = require('../models/groups')
const results = require('../models/results')
const teams = require('../models/teams')
var currentIndex = 0;
var swap = 0;
const addResult = async(req,res,next) => {
    console.log(req.query);
    const teamA = JSON.parse(req.query.teamA)
    const teamB = JSON.parse(req.query.teamB)
    try{
    const newResultA = new results({
        name : teamA.name,
        controller: teamA.controller,
        school: teamA.school,
        score: teamA.score,
        time: teamA.time
    })
    const newResultB = new results({
        name : teamB.name,
        controller: teamB.controller,
        school: teamB.school,
        score: teamB.score,
        time: teamB.time
    })
    await newResultA.save();
    await newResultB.save();
    
    res.json({message:"Check Group Successfully"})
    console.log("Add Group Successfully")
    }
    catch (err) {
        res.json({message:err})
    }
}

const addGroup = async(req,res,next) => {
    console.log(req.body.name);
    try{
    const newGroup = new groups({
        name : req.body.name,
        index: req.body.index,
        idTeamA: req.body.idTeamA,
        idTeamB: req.body.idTeamB
    })
    await newGroup.save()
    
    res.json({message:"Check Group Successfully"})
    console.log("Add Group Successfully - Name : " + newGroup.name)
    }
    catch (err) {
        res.json({message:err})
    }
}
const deleteGroup = async(req,res,next) => {
    let groupName = req.body.name
    await groups.findOneAndRemove({name: groupName})
    .then (async()=>{
        res.json({message:"Delete group Successfully"})
        console.log("Delete group Successfully - Name : " + groupName)
    })
    .catch ((err)=> {
        res.status(500).json({error:err})
        res.json({message:"An Error Occured"})
    })
    
}
const updateGroup = async(req,res,next) => 
{  
    const groupName = req.body.name
    groups.findOne({name: groupName}, (err, doc) => {
        if (doc)
        {
            let updateData = ({
                name: req.body.name,
                index: req.body.index == "" ? doc.index : req.body.index,
                idTeamA: req.body.idTeamA == "" ? doc.idTeamA : req.body.idTeamA,
                idTeamB: req.body.idTeamB == "" ? doc.idTeamB : req.body.idTeamB
            })
            groups.findOneAndUpdate({name:groupName}, {$set:updateData})
            .then (()=>{
                res.json({message:"Update group Successfully"})
                console.log("Update group successfully [] Name : " + groupName)
            })
            .catch ((err)=> {
                res.json({message:"An Error Occured"})
            })
        }
    })
}
const addTeam = async(req,res,next) => {
    // console.log(req.body.id);
    try{
    const newTeam = new teams({
        id : req.body.id,
        name: req.body.name,
        controller: req.body.controller,
        school: req.body.school
    })
    await newTeam.save()
    
    res.json({message:"Check Team Successfully"})
    console.log("Add Team Successfully - Name : " + newTeam.name)
    }
    catch (err) {
        res.json({message:err})
    }
}
const deleteTeam = async(req,res,next) => {
    let teamId = req.body.id
    await teams.findOneAndRemove({id: teamId})
    .then (async()=>{
        res.json({message:"Delete team Successfully"})
        console.log("Delete team Successfully - Id : " + teamId)
    })
    .catch ((err)=> {
        res.status(500).json({error:err})
        res.json({message:"An Error Occured"})
    })
    
}
const updateTeam = async(req,res,next) => 
{  
    const teamId = req.body.id
    teams.findOne({id: teamId}, (err, doc) => {
        if (doc)
        {
            let updateData = ({
                id: req.body.id,
                name: req.body.name == "" ? doc.name : req.body.name,
                controller: req.body.controller == "" ? doc.controller : req.body.controller,
                school: req.body.school == "" ? doc.school : req.body.school
            })
            teams.findOneAndUpdate({id:teamId}, {$set:updateData})
            .then (()=>{
                res.json({message:"Update team Successfully"})
                console.log("Update team successfully [] Id : " + teamId)
            })
            .catch ((err)=> {
                res.json({message:"An Error Occured"})
            })
        }
    })
}
const getSwapTeam = async(req,res,next) => {
    try
    {   
        swap = swap == 1 ? 0 : 1;
        groups.findOne({index: currentIndex}, (err, group) => {
            teams.findOne({id: group.idTeamA}, (err, resTeamA) => {
                teams.findOne({id: group.idTeamB}, (err, resTeamB) => {
                    var respond = {
                        teamA: {
                            name: swap == 1 ? resTeamB.name : resTeamA.name,
                            controller: swap == 1 ? resTeamB.controller : resTeamA.controller,
                            school: swap == 1 ? resTeamB.school : resTeamA.school
                        },
                        teamB: {
                            name: swap == 1 ? resTeamA.name : resTeamB.name,
                            controller: swap == 1 ? resTeamA.controller : resTeamB.controller,
                            school: swap == 1 ? resTeamA.school : resTeamB.school
                        }
                    }
                    res.status(200).json(respond)
                })
            })
        })
    }
    catch (err) {
        res.json({message:"Error"})
    }  
}
const getPreviousTeam = async(req,res,next) => {
    if (currentIndex <= 1) 
        currentIndex = 2
    try
    {   
        swap = 0;
        groups.findOne({index: --currentIndex}, (err, group) => {
            teams.findOne({id: group.idTeamA}, (err, resTeamA) => {
                teams.findOne({id: group.idTeamB}, (err, resTeamB) => {
                    var respond = {
                        teamA: {
                            name: resTeamA.name,
                            controller: resTeamA.controller,
                            school: resTeamA.school
                        },
                        teamB: {
                            name: resTeamB.name,
                            controller: resTeamB.controller,
                            school: resTeamB.school
                        }
                    }
                    res.status(200).json(respond)
                })
            })
        })
    }
    catch (err) {
        res.json({message:"Error"})
    }  
}
const getCurrentTeam = async(req,res,next) => {
    try
    {   
        swap = 0;
        groups.findOne({index: currentIndex}, (err, group) => {
            teams.findOne({id: group.idTeamA}, (err, resTeamA) => {
                teams.findOne({id: group.idTeamB}, (err, resTeamB) => {
                    var respond = {
                        teamA: {
                            name: resTeamA.name,
                            controller: resTeamA.controller,
                            school: resTeamA.school
                        },
                        teamB: {
                            name: resTeamB.name,
                            controller: resTeamB.controller,
                            school: resTeamB.school
                        }
                    }
                    res.status(200).json(respond)
                })
            })
        })
    }
    catch (err) {
        res.json({message:"Error"})
    }  
}
const getNextTeam = async(req,res,next) => {
    try
    {  
        swap = 0;
        groups.findOne({index: ++currentIndex}, (err, group) => {
            teams.findOne({id: group.idTeamA}, (err, resTeamA) => {
                teams.findOne({id: group.idTeamB}, (err, resTeamB) => {
                    var respond = {
                        teamA: {
                            name: resTeamA.name,
                            controller: resTeamA.controller,
                            school: resTeamA.school
                        },
                        teamB: {
                            name: resTeamB.name,
                            controller: resTeamB.controller,
                            school: resTeamB.school
                        }
                    }
                    res.status(200).json(respond)
                })
            })
        })
    }
    catch (err) {
        res.json({message:"Error"})
    }  
}

const getPreviousTeam1 = async(req,res,next) => {
    if (currentIndex <= 1) 
        currentIndex = 2
    try
    {   
        teams.findOne({id: --currentIndex}, (err, resTeam) => {
            var respond = {
                    teamName: resTeam.name,
                    controller: resTeam.controller,
                    school: resTeam.school
            }
            res.status(200).json(respond)
        })
    }
    catch (err) {
        res.json({message:"Error"})
    }  
}
const getCurrentTeam1 = async(req,res,next) => {
    try
    {   
        teams.findOne({id: currentIndex}, (err, resTeam) => {
            var respond = {
                    teamName: resTeam.name,
                    controller: resTeam.controller,
                    school: resTeam.school
            }
            res.status(200).json(respond)
        })
    }
    catch (err) {
        res.json({message:"Error"})
    }
}
const getNextTeam1 = async(req,res,next) => {
    try
    {   
        teams.findOne({id: ++currentIndex}, (err, resTeam) => {
            var respond = {
                    teamName: resTeam.name,
                    controller: resTeam.controller,
                    school: resTeam.school
            }
            res.status(200).json(respond)
        })
    }
    catch (err) {
        res.json({message:"Error"})
    }
}
const addResult1 = async(req,res,next) => {
    // console.log(req.query);
    // const teamA = JSON.parse(req.query.teamA)

    try{
        teams.findOne({id: currentIndex}, async(err, resTeam) => {
            const newResult = new results({
                name: resTeam.name,
                controller: resTeam.controller,
                school: resTeam.school,
                score: req.query.totalscore,
                time: req.query.finishtime
            })
            await newResult.save();
            res.json({message:"Save Result Successfully"})
            console.log("Save Result Successfully")
        })
    }
    catch (err) {
        res.json({message:err})
    }
}
module.exports = {addGroup, deleteGroup, updateGroup, addTeam, deleteTeam, updateTeam,
    getPreviousTeam, getCurrentTeam, getNextTeam, getSwapTeam, addResult,
    getPreviousTeam1, getCurrentTeam1, getNextTeam1, addResult1}

const express = require('express')
const router = express.Router()
const controller  = require('../controllers/controller')

module.exports = function (io) {
router.get("/",(req,res)=> res.render("../views/home.ejs"))
router.get("/control",async (req,res)=> {
    res.render("../views/control.ejs")
})
router.post('/addGroup',controller.addGroup)
router.post('/deleteGroup', controller.deleteGroup)
router.post('/updateGroup', controller.updateGroup)
router.post('/addTeam',controller.addTeam)
router.post('/deleteTeam', controller.deleteTeam)
router.post('/updateTeam', controller.updateTeam)
router.get('/previousteam', controller.getPreviousTeam)
router.get('/currentteam', controller.getCurrentTeam)
router.get('/nextteam', controller.getNextTeam)
router.get('/swap', controller.getSwapTeam)
router.get('/savedb',controller.addResult)
return router;

}
    
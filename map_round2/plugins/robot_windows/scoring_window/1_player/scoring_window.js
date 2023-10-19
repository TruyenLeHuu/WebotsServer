const stopwatch_time = document.getElementById("stopwatch-time");
const newCheckPoint = document.getElementById("generate-checkpoint");
const resultScore = document.getElementById("result-score");
const _teamName = document.getElementById("team-name");
const resultFinishTime = document.getElementById("result-finishtime")
const saveDB = document.getElementById("save-db");
const controllerName = document.getElementById("controller-namee");

const numberOfCheckpoint = 5;

var jsonTeam = null;
var totalScrore = 0;
var current_cp = 1;
var finishTime = "00:00.000";
var rawTime = 0;
var isOutline = false;

function LoadCurrentTeam() {
	var xmlHttp = new XMLHttpRequest();
	var URL = `http://localhost:3001/currentteam1`;
	xmlHttp.open("GET", URL);
	xmlHttp.send(null);
	flag = true;
	xmlHttp.onreadystatechange = (e) => {
		var str = xmlHttp.responseText;
		if (str.length != 0) {
			UpdateTeam(str);
		}
	}
}

// Window onload
window.onload = function () {
	window.robotWindow = webots.window("scoring_window");
	LoadCurrentTeam();
	window.robotWindow.receive = function (value, robot) {
		let arr = value.split('|');

		let flag = arr[0];
		let time = arr[1];
		UpdateStopwatch(FormatTime(time));

		if (flag.split(".")[0] == "C") {
			updateCPindex(parseInt(flag.split('.')[1]));
			Passed(current_cp);
		}
		else if (flag.split(".")[0] == "O") {
			Outline();
		}
	}
}

function updateCPindex(index) {
	current_cp = index;
}

function FormatTime(strTime) { //double
	let arr = strTime.split(".");

	let min = (parseInt(arr[0]) / 60).toString().split('.')[0];
	let sec = (parseInt(arr[0]) % 60).toString();
	let miSec = arr[1].substring(0, 3);

	if (min.length < 2)
		min = "0" + min;
	if (sec.length < 2)
		sec = "0" + sec;
	rawTime = min*60*1000+sec*1000+miSec;
	let time = min + ":" + sec + "." + miSec;
	return time;
}

function UpdateStopwatch(time) {
	stopwatch_time.innerHTML = time;
}

// Auto generate check point
function GenerateCheckpoint() {
	for (var i = 1; i <= numberOfCheckpoint; i++) {
		var checkpoint = document.createElement("div");
		checkpoint.setAttribute("class", "checkpoint not-pass");
		var cp_id = "cp" + i;
		checkpoint.setAttribute("id", cp_id)

		var cp_name = document.createElement("h3");
		cp_name.innerHTML = "CHECK POINT " + i;

		var cp_time = document.createElement("h3");
		var time_id = "checkpoin-time" + i;
		cp_time.setAttribute("id", time_id);
		cp_time.innerHTML = "--:--.---";

		checkpoint.appendChild(cp_name);
		checkpoint.appendChild(cp_time);
		newCheckPoint.appendChild(checkpoint);
	}
}
GenerateCheckpoint();

function Start() {
	console.log("START");
	isOutline = false;
	finishTime = "00:00.000";
	window.robotWindow.send('start');
}


function Reset() {
	ResetCheckpoint();
	totalScrore = 0;
	finishTime = "00:00.000";
	isOutline = false;
	window.robotWindow.send('reload');
}

var flag = true;
function UpdateTeam(str) {
	if (flag) {
		jsonTeam = JSON.parse(str);
		_teamName.innerHTML = jsonTeam.teamName;
		controllerName.innerHTML = "Controller: " + jsonTeam.controller;
		window.robotWindow.send(jsonTeam.controller);
		flag = false;
	}
}

function LoadNextTeam() {
	var xmlHttp = new XMLHttpRequest();
	var URL = `http://localhost:3001/nextteam1`;
	xmlHttp.open("GET", URL);
	xmlHttp.send(null);
	flag = true;
	xmlHttp.onreadystatechange = (e) => {
		var str = xmlHttp.responseText;
		if (str.length != 0) {
			UpdateTeam(str);
		}
	}
}

function LoadPreviousTeam() {
	var xmlHttp = new XMLHttpRequest();
	var URL = `http://localhost:3001/previousteam1`;
	xmlHttp.open("GET", URL);
	xmlHttp.send(null);
	flag = true;
	xmlHttp.onreadystatechange = (e) => {
		var str = xmlHttp.responseText;
		if (str.length != 0) {
			UpdateTeam(str);
		}
	}
}

// Save result
function SaveResult2DB() {
	var xmlHttp = new XMLHttpRequest();
	var URL = `http://localhost:3001/savedb1?totalscore=${totalScrore}&finishtime=${finishTime}&rawtime=${rawTime}`;
	xmlHttp.open("GET", URL);
	xmlHttp.send(null);
	xmlHttp.onreadystatechange = (e) => {
		console.log(xmlHttp.responseText);
	}
}

function Passed(cp_index) {
	if (isOutline == true) return;
	if (cp_index <= numberOfCheckpoint) {
		totalScrore++;
		resultScore.innerHTML = totalScrore;
		SetCheckPointPassed(cp_index);
		if (cp_index == numberOfCheckpoint) {
			resultFinishTime.innerHTML = finishTime;
			return;
		}
	}
}

var flagStop = false;
function CheckpointPassed(index) {
	if (flagStop == true) return;
	if (index <= numberOfCheckpoint) {
		totalScrore++;
		resultScore.innerHTML = totalScrore;
		SetCheckPointPassed(index);
		if (index == numberOfCheckpoint) {
			resultFinishTime.innerHTML = finishTime;
			flagStop = true;
			return;
		}
	}
}

function SetCheckPointPassed(index) {
	if (index > numberOfCheckpoint) return;
	// update status
	var id = "cp" + index;
	document.getElementById(id).setAttribute("class", "checkpoint pass");
	// update time
	var time_id = "checkpoin-time" + index;
	var time = stopwatch_time.innerHTML;
	document.getElementById(time_id).innerHTML = time;
	finishTime = time;
}

function ResetCheckpoint() {
	for (var index = 1; index <= numberOfCheckpoint; index++) {
		// update status
		var id = "cp" + index;
		document.getElementById(id).setAttribute("class", "checkpoint not-pass");
		// update time
		var time = "--:--.---";
		var time_id = "checkpoin-time" + index;
		document.getElementById(time_id).innerHTML = time;
	}
}

function Outline() {
	resultFinishTime.innerHTML = finishTime;
	isOutline = true;
	for (var index = current_cp + 1; index <= numberOfCheckpoint; index++) {
		// update status
		var id = "cp" + index;
		// console.log(id);
		document.getElementById(id).setAttribute("class", "checkpoint outline");
	}
}

function Cam1() {
	console.log("Cam1");
	window.robotWindow.send('cam1');
}
function Cam2() {
	console.log("Cam2");
	window.robotWindow.send('cam2');
}
function Cam3() {
	console.log("Cam3");
	window.robotWindow.send('cam3');
}
function Cam4() {
	console.log("Cam4");
	window.robotWindow.send('cam4');
}
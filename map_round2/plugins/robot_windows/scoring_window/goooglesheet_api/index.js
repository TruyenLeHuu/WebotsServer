const express = require("express");
const { google } = require("googleapis");

const app = express();

app.get("/", async (req, res) => {
    const auth = new google.auth.GoogleAuth({
        keyFile: "credential.json",
        scopes: "https://www.googleapis.com/auth/spreadsheets",
      });
    
      // Create client instance for auth
      const client = await auth.getClient();
    
      // Instance of Google Sheets API
      const googleSheets = google.sheets({ version: "v4", auth: client });

      //https://docs.google.com/spreadsheets/d/1J-nzhT3Q6pMfEbZllM7OFyj5zqxJWeH9hRa2qWcV128/edit#gid=0
      const spreadsheetId = "1J-nzhT3Q6pMfEbZllM7OFyj5zqxJWeH9hRa2qWcV128";
    
      // Get metadata about spreadsheet
      const metaData = await googleSheets.spreadsheets.get({
        auth,
        spreadsheetId,
      });

      const getRows = await googleSheets.spreadsheets.values.get({
        auth,
        spreadsheetId,
        range: "Sheet1!1:1",
      });
      var test = String(getRows.data.values).split(',');
      
      res.send(getRows.data.values);
    
      
});

app.get("/append", async (req, res) => {
  const auth = new google.auth.GoogleAuth({
      keyFile: "credential.json",
      scopes: "https://www.googleapis.com/auth/spreadsheets",
    });
  
    // Create client instance for auth
    const client = await auth.getClient();
  
    // Instance of Google Sheets API
    const googleSheets = google.sheets({ version: "v4", auth: client });

    //https://docs.google.com/spreadsheets/d/1J-nzhT3Q6pMfEbZllM7OFyj5zqxJWeH9hRa2qWcV128/edit#gid=0
    const spreadsheetId = "1J-nzhT3Q6pMfEbZllM7OFyj5zqxJWeH9hRa2qWcV128";
  
  
    const response = await googleSheets.spreadsheets.values.append({
      spreadsheetId,
      auth,
      range: "Sheet2!1:1",
      valueInputOption: "RAW",
      resource: {
         values:  [['a', 'b', 'c'],['d', 'e', 'f']]
      }
   });    
  res.send("Success!");
});


app.listen(3000, (req, res) => console.log("running on 3000"));

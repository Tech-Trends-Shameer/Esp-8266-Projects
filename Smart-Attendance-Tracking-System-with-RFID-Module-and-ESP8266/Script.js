//Tech Trends Shameer
//Smart Attendance System

var ss = SpreadsheetApp.openById('SheetID');
var sheet = ss.getSheetByName('Sheet1');
var timezone = 'Asia/Karachi'; // Corrected timezone format

// ------------------------------------------------------------------------
function doGet(e){
  Logger.log(JSON.stringify(e));

  // Check if the 'name' parameter is undefined
  if (typeof e.parameter.name === 'undefined') {
    return ContentService.createTextOutput('Received data is undefined');
  }
  
  var Curr_Date = new Date();
  var Curr_Time = Utilities.formatDate(Curr_Date, timezone, 'HH:mm:ss');
  var name = stripQuotes(e.parameter.name);
  
  var nextRow = sheet.getLastRow() + 1;
  sheet.getRange('A' + nextRow).setValue(Curr_Date);
  sheet.getRange('B' + nextRow).setValue(Curr_Time);
  sheet.getRange('C' + nextRow).setValue(name);
  
  // Returns response back to ESP32
  var response = 'Card holder name is stored in column C';
  return ContentService.createTextOutput(response);
}

// ------------------------------------------------------------------------
function stripQuotes(value) {
  return value.toString().replace(/^["']|['"]$/g, '');
}

// ------------------------------------------------------------------------
// Extra Function. Not used in this project.
// Planning to use in future projects.
// This function is used to handle POST request
function doPost(e) {
  var val = e.parameter.value;
  
  if (e.parameter.value !== undefined){
    var range = sheet.getRange('A2');
    range.setValue(val);
  }
}
// ------------------------------------------------------------------------

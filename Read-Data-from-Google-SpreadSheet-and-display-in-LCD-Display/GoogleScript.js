var ss = SpreadsheetApp.openById('1sv00-AtvAAW6x50stmrCjl_grCvHIxI_5qbv44LzRrffsgdgvU');
var sheet = ss.getSheetByName('Sheet1');


function doGet(e){
  var read = e.parameter.read;

  if (read !== undefined){
    return ContentService.createTextOutput(sheet.getRange('A1').getValue());
  }
}

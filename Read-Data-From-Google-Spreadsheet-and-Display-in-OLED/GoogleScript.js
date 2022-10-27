 
var ss = SpreadsheetApp.openById('192k4yM-OrKsxU4E9w7pjkdk0OsdghhffjyjkkkMbV2k0HZARcqkNzI8g');
var sheet = ss.getSheetByName('Sheet1');
 
function doGet(e){
  var read = e.parameter.read;

  if (read !== undefined){
    return ContentService.createTextOutput(sheet.getRange('A1').getValue());
  }
}

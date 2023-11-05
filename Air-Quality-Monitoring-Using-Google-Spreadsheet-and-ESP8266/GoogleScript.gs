
var SS = SpreadsheetApp.openById('1uRJJVWRdGQ-ll5dGdxywYO8lxJVfplRWWEkeul1e1_M'); // Enter Your Sheet ID Got From Sheet URL Link
var sheet = SS.getSheetByName('air_quality_data_sheet'); // Enter your sheet name here, In my case it is Temp_Data_Sheet
var str = "";

function onOpen() {
  var ui = SpreadsheetApp.getUi();
  ui.createMenu('Air_Quality_Monitoring')
    .addItem('Clear', 'Clear')
    .addToUi();
}

function Clear() {
  sheet.deleteRows(4, sheet.getLastRow());
  SS.toast('Chart cleared', 'Air_Quality_Monitoring', 5);
}

function doPost(e) {
  var parsedData;
  var result = {};

  try {
    parsedData = JSON.parse(e.postData.contents);
  } catch (f) {
    return ContentService.createTextOutput("Error in parsing request body: " + f.message);
  }

  if (parsedData !== undefined) {
    var flag = parsedData.format;

    if (flag === undefined) {
      flag = 0;
    }

    switch (parsedData.command) {
      case "appendRow":
        var tmp = SS.getSheetByName(parsedData.sheet_name);
        var nextFreeRow = tmp.getLastRow() + 1;
        var dataArr = parsedData.values.split(",");

        // Add date, temperature, humidity, and air quality to the data array
        var now = new Date();
        dataArr.unshift(Utilities.formatDate(now, "EST", "yyyy-MM-dd hh:mm a")); // Format date without 'T' and 'Z'

        tmp.appendRow(dataArr);

        str = "Success";
        SpreadsheetApp.flush();
        break;
    }

    return ContentService.createTextOutput(str);
  } else {
    return ContentService.createTextOutput("Error! Request body empty or in incorrect format.");
  }
}

function doGet(e) {
  var val = e.parameter.value;
  var cal = e.parameter.cal;
  var read = e.parameter.read;

  if (cal !== undefined) {
    return ContentService.createTextOutput(GetEventsOneWeek());
  }

  if (read !== undefined) {
    var now = Utilities.formatDate(new Date(), "EST", "yyyy-MM-dd hh:mm a");
    sheet.getRange('C1').setValue(count);
    return ContentService.createTextOutput(sheet.getRange('A1').getValue());
  }

  if (e.parameter.value === undefined)
    return ContentService.createTextOutput("No value passed as an argument to the script URL.");

  var range = sheet.getRange('A1');
  var retval = range.setValue(val).getValue();
  var now = Utilities.formatDate(new Date(), "EST", "yyyy-MM-dd hh:mm a");
  sheet.getRange('A1').setValue("Date");
  sheet.getRange('B1').setValue(val); // Assuming 'val' is the temperature
  sheet.getRange('C1').setValue("Humidity");
  sheet.getRange('D1').setValue("Air Quality");

  if (retval == e.parameter.value)
    return ContentService.createTextOutput("Successfully wrote: " + e.parameter.value + "\ninto the spreadsheet.");
  else
    return ContentService.createTextOutput("Unable to write into the spreadsheet.\nCheck authentication and make sure the cursor is not on cell 'A1'. " + retval + ' ' + e.parameter.value);
}

function GetEventsOneWeek() {
  var Cal = CalendarApp.getCalendarsByName('Test REST API')[0];
  var Now = new Date();
  var OneWeekFromNow = new Date();
  OneWeekFromNow.setDate(Now.getDate() + 7);
  var events = Cal.getEvents(Now, OneWeekFromNow);
  var str = '\nEvent Title,\tDescription,\tRecurring?,\tAll-day?,\tFirst Reminder (in minutes before event)\n';
  for (var i = 0; i < events.length; i++) {
    str += events[i].getTitle() + ',\t' + events[i].getDescription() + ',\t' + events[i].isRecurringEvent() + ',\t' + events[i].isAllDayEvent() + ',\t' + events[i].getPopupReminders()[0];
    str += '\n';
  }
  return str;
}

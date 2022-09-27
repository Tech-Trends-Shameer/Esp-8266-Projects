<?php
//-------------------------------------------------------------------------------------------
require 'config.php';

$db;
	$sql = "SELECT * FROM tbl_temperature ORDER BY id DESC LIMIT 30";
	$result = $db->query($sql);
	if (!$result) {
	  { echo "Error: " . $sql . "<br>" . $db->error; }
	}

	//$rows = $result->fetch_assoc();
	//$rows = $result -> fetch_all(MYSQLI_ASSOC);

//$row = get_temperature();
//print_r($row);

//header('Content-Type: application/json');
//echo json_encode($rows);
//-------------------------------------------------------------------------------------------
?>




<!DOCTYPE html>
<html lang="en">
<head>
  <title>Bootstrap Example</title>
  <meta charset="utf-8">
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="stylesheet" href="https://cdn.jsdelivr.net/npm/bootstrap@4.6.1/dist/css/bootstrap.min.css">
  <script src="https://code.jquery.com/jquery-3.1.1.min.js"></script>
  <script src="https://cdn.jsdelivr.net/npm/popper.js@1.16.1/dist/umd/popper.min.js"></script>
  <script src="https://cdn.jsdelivr.net/npm/bootstrap@4.6.1/dist/js/bootstrap.bundle.min.js"></script>


<style>
.chart {
  width: 100%; 
  min-height: 450px;
}
.row {
  margin:0 !important;
}
</style>
   
</head>
<body>
  
<div class="container">
  
  <div class="row">
  <div class="col-md-12 text-center">
    <h1>Real Time Weather Station</h1>
    <p>Created By: <a href="#">AhmadLogs</a></p>
  </div>
  <div class="clearfix"></div>
  
  
  <div class="col-md-6">
    <div id="chart_temperature" class="chart"></div>
  </div>
  
  
  <div class="col-md-6">
    <div id="chart_humidity" class="chart"></div>
  </div>

</div>


<div class="row">
  <div class="col-md-12">
    <table class="table">
    <thead>
      <tr>
        <th scope="col">#</th>
        <th scope="col">Temperature</th>
        <th scope="col">Humidity</th>
        <th scope="col">date time</th>
      </tr>
    </thead>
    <tbody>
    <?PHP $i = 1; while ($row = mysqli_fetch_assoc($result)) {?>
      <tr>
        <th scope="row"><?php echo $i++;?></th>
        <td><?PHP echo $row['temperature'];?></td>
        <td><?PHP echo $row['humidity'];?></td>
        <td><?PHP echo date("Y-m-d h:i: A", strtotime($row['created_date']));?></td>
      </tr>
    <?PHP } ?>
    </tbody>
  </table>
</div>
</div>
</div>
<!-- ---------------------------------------------------------------------------------------- -->
 
<script type="text/javascript" src="https://www.gstatic.com/charts/loader.js"></script>
<script>
//$(document).ready(function(){
//-------------------------------------------------------------------------------------------------
google.charts.load('current', {'packages':['gauge']});
google.charts.setOnLoadCallback(drawTemperatureChart);
//-------------------------------------------------------------------------------------------------
function drawTemperatureChart() {
	//guage starting values
	var data = google.visualization.arrayToDataTable([
		['Label', 'Value'],
		['Temperature', 0],
	]);
	//NNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNN
	var options = {
		width: 		1600, 
		height: 	480,
		redFrom: 	70, 
		redTo:		100,
		yellowFrom:	40, 
		yellowTo: 	70,
		greenFrom:	00, 
		greenTo: 	40,
		minorTicks: 5
	};
	//NNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNN
	var chart = new google.visualization.Gauge(document.getElementById('chart_temperature'));
	chart.draw(data, options);
	//NNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNN



	//NNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNN
	function refreshData () {
		$.ajax({
			url: 'getdata.php',
			// use value from select element
			data: 'q=' + $("#users").val(),
			dataType: 'json',
			success: function (responseText) {
				//______________________________________________________________
				//console.log(responseText);
				var var_temperature = parseFloat(responseText.temperature).toFixed(2)
				//console.log(var_temperature);
				// use response from php for data table
				//______________________________________________________________
				//guage starting values
				var data = google.visualization.arrayToDataTable([
					['Label', 'Value'],
					['Temperature', eval(var_temperature)],
				]);
				//______________________________________________________________
				//var chart = new google.visualization.Gauge(document.getElementById('chart_temperature'));
				chart.draw(data, options);
			},
			error: function(jqXHR, textStatus, errorThrown) {
				console.log(errorThrown + ': ' + textStatus);
			}
		});
    }
	//NNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNN
	//refreshData();
	
	setInterval(refreshData, 1000);
}
//-------------------------------------------------------------------------------------------------



//-------------------------------------------------------------------------------------------------
google.charts.load('current', {'packages':['gauge']});
google.charts.setOnLoadCallback(drawHumidityChart);
//-------------------------------------------------------------------------------------------------
function drawHumidityChart() {
	//guage starting values
	var data = google.visualization.arrayToDataTable([
		['Label', 'Value'],
		['Humidity', 0],
	]);
	//NNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNN
	var options = {
		width: 		1600, 
		height: 	480,
		redFrom: 	70, 
		redTo:		100,
		yellowFrom:	40, 
		yellowTo: 	70,
		greenFrom:	00, 
		greenTo: 	40,
		minorTicks: 5
	};
	//NNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNN
	var chart = new google.visualization.Gauge(document.getElementById('chart_humidity'));
	chart.draw(data, options);
	//NNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNN



	//NNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNN
	function refreshData () {
		$.ajax({
			url: 'getdata.php',
			// use value from select element
			data: 'q=' + $("#users").val(),
			dataType: 'json',
			success: function (responseText) {
				//______________________________________________________________
				//console.log(responseText);
				var var_humidity = parseFloat(responseText.humidity).toFixed(2)
				//console.log(var_temperature);
				// use response from php for data table
				//______________________________________________________________
				//guage starting values
				var data = google.visualization.arrayToDataTable([
					['Label', 'Value'],
					['Humidity', eval(var_humidity)],
				]);
				//______________________________________________________________
				//var chart = new google.visualization.Gauge(document.getElementById('chart_temperature'));
				chart.draw(data, options);
			},
			error: function(jqXHR, textStatus, errorThrown) {
				console.log(errorThrown + ': ' + textStatus);
			}
		});
    }
	//NNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNN
	//refreshData();
	
	setInterval(refreshData, 1000);
}
//-------------------------------------------------------------------------------------------------

//});




$(window).resize(function(){
  drawTemperatureChart();
  drawHumidityChart();
});





</script>
<!-- --------------------------------------------------------------------- -->
</body>
</html>

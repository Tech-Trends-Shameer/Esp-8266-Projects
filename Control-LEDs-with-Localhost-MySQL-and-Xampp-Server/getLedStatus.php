<?php
$servername = "localhost";
$username = "root";
$password = "";
$dbname = "control_led";

// Create connection
$conn = new mysqli($servername, $username, $password, $dbname);

// Check connection
if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error);
}

//retrieve LED status
$sql = "SELECT * FROM led_status WHERE id = 1";
$result = $conn->query($sql);

if ($result) {
    if ($result->num_rows > 0) {
        $row = $result->fetch_assoc();
        echo json_encode($row);
    } else {
        echo json_encode(["error" => "No data found"]);
    }
} else {
    echo json_encode(["error" => "Query failed"]);
}

// Close the connection
$conn->close();
?>

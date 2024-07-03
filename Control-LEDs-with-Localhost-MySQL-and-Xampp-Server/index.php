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

// Handle AJAX request
if (isset($_POST['led']) && isset($_POST['status'])) {
    $led = $_POST['led'];
    $status = $_POST['status'];

    $sql = "UPDATE led_status SET $led=$status WHERE id=1";
    $conn->query($sql);
    exit;
}

// Retrieve LED status
$sql = "SELECT * FROM led_status WHERE id=1";
$result = $conn->query($sql);
$row = $result->fetch_assoc();
?>

<!DOCTYPE html>
<html>
<head>
    <title>ESP32 LED Control</title>
    <style>
        @import url('https://fonts.googleapis.com/css2?family=Poppins:wght@400;600&display=swap');

        body {
            font-family: 'Poppins', sans-serif;
            background: linear-gradient(135deg, #72edf2 10%, #5151e5 100%);
            margin: 0;
            padding: 0;
            display: flex;
            justify-content: center;
            align-items: center;
            height: 100vh;
        }
        .container {
            background: #ffffff;
            padding: 40px;
            border-radius: 15px;
            box-shadow: 0 10px 20px rgba(0, 0, 0, 0.2);
            width: 350px;
            text-align: center;
        }
        .brand-name {
            font-size: 24px;
            font-weight: 600;
            color: #00796b;
            margin-bottom: 10px;
        }
        h1 {
            color: #333;
            margin-bottom: 20px;
            font-size: 28px;
        }
        .toggle-switch {
            position: relative;
            display: inline-block;
            width: 70px;
            height: 38px;
            margin: 15px 0;
        }
        .toggle-switch input {
            opacity: 0;
            width: 0;
            height: 0;
        }
        .slider {
            position: absolute;
            cursor: pointer;
            top: 0;
            left: 0;
            right: 0;
            bottom: 0;
            background-color: #ccc;
            transition: .4s;
            border-radius: 38px;
        }
        .slider:before {
            position: absolute;
            content: "";
            height: 30px;
            width: 30px;
            left: 4px;
            bottom: 4px;
            background-color: white;
            transition: .4s;
            border-radius: 50%;
        }
        input:checked + .slider {
            background-color: #4CAF50;
        }
        input:checked + .slider:before {
            transform: translateX(32px);
        }
        .led-label {
            display: flex;
            align-items: center;
            justify-content: space-between;
            margin-bottom: 15px;
            font-size: 20px;
            color: #555;
        }
    </style>
    <script>
        function toggleLED(led, status) {
            var xhr = new XMLHttpRequest();
            xhr.open("POST", "", true);
            xhr.setRequestHeader("Content-Type", "application/x-www-form-urlencoded");
            xhr.send("led=" + led + "&status=" + status);
        }

        function handleToggle(event) {
            var led = event.target.name;
            var status = event.target.checked ? 1 : 0;
            toggleLED(led, status);
        }
    </script>
</head>
<body>
    <div class="container">
        <div class="brand-name">Tech Trends Shameer</div>
        <h1>ESP32 LED Control</h1>
        <div class="led-label">
            <span>LED 1</span>
            <label class="toggle-switch">
                <input type="checkbox" name="led1" <?php if ($row['led1']) echo 'checked'; ?> onchange="handleToggle(event)">
                <span class="slider"></span>
            </label>
        </div>
        <div class="led-label">
            <span>LED 2</span>
            <label class="toggle-switch">
                <input type="checkbox" name="led2" <?php if ($row['led2']) echo 'checked'; ?> onchange="handleToggle(event)">
                <span class="slider"></span>
            </label>
        </div>
        <div class="led-label">
            <span>LED 3</span>
            <label class="toggle-switch">
                <input type="checkbox" name="led3" <?php if ($row['led3']) echo 'checked'; ?> onchange="handleToggle(event)">
                <span class="slider"></span>
            </label>
        </div>
        <div class="led-label">
            <span>LED 4</span>
            <label class="toggle-switch">
                <input type="checkbox" name="led4" <?php if ($row['led4']) echo 'checked'; ?> onchange="handleToggle(event)">
                <span class="slider"></span>
            </label>
        </div>
    </div>
</body>
</html>

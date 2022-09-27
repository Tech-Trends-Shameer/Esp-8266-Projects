<?php 
define('DB_HOST'    , 'localhost'); 
define('DB_USERNAME', 'Database Usernamer'); 
define('DB_PASSWORD', 'Database Password'); 
define('DB_NAME'    , 'Database Name');

define('POST_DATA_URL', 'ENTER_YOUR_DOMAIN_NAME/sensordata.php');

//PROJECT_API_KEY is the exact duplicate of, PROJECT_API_KEY in NodeMCU sketch file
//Both values must be same
define('PROJECT_API_KEY', '5fsdsf2ev5FF');


//set time zone for your country
date_default_timezone_set('Asia/Karachi');

// Connect with the database 
$db = new mysqli(DB_HOST, DB_USERNAME, DB_PASSWORD, DB_NAME); 
 
// Display error if failed to connect 
if ($db->connect_errno) { 
    echo "Connection to database is failed: ".$db->connect_error;
    exit();
}

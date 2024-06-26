<?php
$servername = "climateguard.local";
$username = "root";
$password = "root";
$dbname = "ClimateGuard";

// Create connection
$conn = new mysqli($servername, $username, $password, $dbname);

// Check connection
if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error);
}

// Update station mode
$station_id = $_POST['station_id'];
$station_mode = $_POST['station_mode'];

// Prepare and execute the update statement
$update_mode_stmt = $conn->prepare("UPDATE WeatherStations SET mode=? WHERE station_id=?");
$update_mode_stmt->bind_param("ss", $station_mode, $station_id);
$update_mode_stmt->execute();

if ($update_mode_stmt->affected_rows > 0) {
    echo "Mode updated successfully";
} else {
    echo "No changes made or invalid station_id";
}

$update_mode_stmt->close();
$conn->close();
?>

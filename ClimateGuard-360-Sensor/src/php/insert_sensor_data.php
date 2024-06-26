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

// Prepare and bind
$station_id = $_POST['station_id'];
$temperature = $_POST['temperature'];
$humidity = $_POST['humidity'];
$pressure = $_POST['pressure'];
$light_intensity = $_POST['light_intensity'];
$air_quality = $_POST['air_quality'];
$rain = $_POST['rain'];

// Prepare statements for each sensor data insertion
$temperature_stmt = $conn->prepare("INSERT INTO Temperatures (station_id, value, date, time) VALUES (?, ?, CURDATE(), CURTIME())");
$humidity_stmt = $conn->prepare("INSERT INTO Humidity (station_id, value, date, time) VALUES (?, ?, CURDATE(), CURTIME())");
$pressure_stmt = $conn->prepare("INSERT INTO Pressure (station_id, value, date, time) VALUES (?, ?, CURDATE(), CURTIME())");
$light_stmt = $conn->prepare("INSERT INTO LightIntensity (station_id, value, date, time) VALUES (?, ?, CURDATE(), CURTIME())");
$air_stmt = $conn->prepare("INSERT INTO AirQuality (station_id, value, date, time) VALUES (?, ?, CURDATE(), CURTIME())");
$rain_stmt = $conn->prepare("INSERT INTO RainSensor (station_id, value, date, time) VALUES (?, ?, CURDATE(), CURTIME())");

$temperature_stmt->bind_param("sd", $station_id, $temperature);
$humidity_stmt->bind_param("sd", $station_id, $humidity);
$pressure_stmt->bind_param("sd", $station_id, $pressure);
$light_stmt->bind_param("sd", $station_id, $light_intensity);
$air_stmt->bind_param("sd", $station_id, $air_quality);
$rain_stmt->bind_param("sd", $station_id, $rain);

$temperature_stmt->execute();
$humidity_stmt->execute();
$pressure_stmt->execute();
$light_stmt->execute();
$air_stmt->execute();
$rain_stmt->execute();

echo "New records created successfully";

$temperature_stmt->close();
$humidity_stmt->close();
$pressure_stmt->close();
$light_stmt->close();
$air_stmt->close();
$rain_stmt->close();
$conn->close();
?>

<?php
$servername = "mysql";
$username = "root";
$password = "root";
$dbname = "ClimateGuard";

// Create connection
$conn = new mysqli($servername, $username, $password, $dbname);

// Check connection
if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error);
}

// Retrieve POST data
$station_id = $_POST['station_id'] ?? null;
$temperature = $_POST['temperature'] ?? null;
$humidity = $_POST['humidity'] ?? null;
$pressure = $_POST['pressure'] ?? null;
$light_intensity = $_POST['light_intensity'] ?? null;
$air_quality = $_POST['air_quality'] ?? null;
$rain = $_POST['rain'] ?? null;
$station_name = $_POST['station_name'] ?? null;
$station_description = $_POST['station_description'] ?? null;
$station_mode = $_POST['station_mode'] ?? null;

// Check if station_id is provided
if (!$station_id) {
    die("Missing required POST data: station_id");
}

// Insert or update sensor data if provided
try {
    if (!is_null($temperature)) {
        $temperature_stmt = $conn->prepare("INSERT INTO Temperatures (station_id, value, date, time) VALUES (?, ?, CURDATE(), CURTIME())");
        $temperature_stmt->bind_param("sd", $station_id, $temperature);
        $temperature_stmt->execute();
        $temperature_stmt->close();
    }

    if (!is_null($humidity)) {
        $humidity_stmt = $conn->prepare("INSERT INTO Humidity (station_id, value, date, time) VALUES (?, ?, CURDATE(), CURTIME())");
        $humidity_stmt->bind_param("sd", $station_id, $humidity);
        $humidity_stmt->execute();
        $humidity_stmt->close();
    }

    if (!is_null($pressure)) {
        $pressure_stmt = $conn->prepare("INSERT INTO Pressure (station_id, value, date, time) VALUES (?, ?, CURDATE(), CURTIME())");
        $pressure_stmt->bind_param("sd", $station_id, $pressure);
        $pressure_stmt->execute();
        $pressure_stmt->close();
    }

    if (!is_null($light_intensity)) {
        $light_stmt = $conn->prepare("INSERT INTO LightIntensity (station_id, value, date, time) VALUES (?, ?, CURDATE(), CURTIME())");
        $light_stmt->bind_param("sd", $station_id, $light_intensity);
        $light_stmt->execute();
        $light_stmt->close();
    }

    if (!is_null($air_quality)) {
        $air_stmt = $conn->prepare("INSERT INTO AirQuality (station_id, value, date, time) VALUES (?, ?, CURDATE(), CURTIME())");
        $air_stmt->bind_param("sd", $station_id, $air_quality);
        $air_stmt->execute();
        $air_stmt->close();
    }

    if (!is_null($rain)) {
        $rain_stmt = $conn->prepare("INSERT INTO RainSensor (station_id, value, date, time) VALUES (?, ?, CURDATE(), CURTIME())");
        $rain_stmt->bind_param("sd", $station_id, $rain);
        $rain_stmt->execute();
        $rain_stmt->close();
    }
} catch (Exception $e) {
    echo "Error inserting sensor data: " . $e->getMessage();
}

// Insert or update station data if provided
if (!is_null($station_name) || !is_null($station_description) || !is_null($station_mode)) {
    try {
        $station_stmt = $conn->prepare("INSERT INTO WeatherStations (station_id, name, description, mode) VALUES (?, ?, ?, ?)
                                        ON DUPLICATE KEY UPDATE name=VALUES(name), description=VALUES(description), mode=VALUES(mode)");
        $station_stmt->bind_param("ssss", $station_id, $station_name, $station_description, $station_mode);
        $station_stmt->execute();
        $station_stmt->close();
    } catch (Exception $e) {
        echo "Error inserting or updating station data: " . $e->getMessage();
    }
}

echo "Data processed successfully";

$conn->close();
?>

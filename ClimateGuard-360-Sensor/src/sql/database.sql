CREATE DATABASE IF NOT EXISTS ClimateGuard;

CREATE TABLE WeatherStations
(
    station_id VARCHAR(17) PRIMARY KEY,
    name VARCHAR(100) NOT NULL,
    description TEXT,
    mode VARCHAR(10) CHECK (mode IN ('Indoor', 'Outdoor', 'Hybrid'))
);

CREATE TABLE Temperatures
(
    temp_id INT AUTO_INCREMENT PRIMARY KEY,
    station_id VARCHAR(17),
    value DECIMAL(5, 2),
    date DATE,
    time TIME,
    FOREIGN KEY (station_id) REFERENCES WeatherStations(station_id)
);

CREATE TABLE Humidity
(
    humidity_id INT AUTO_INCREMENT PRIMARY KEY,
    station_id VARCHAR(17),
    value DECIMAL(5, 2),
    date DATE,
    time TIME,
    FOREIGN KEY (station_id) REFERENCES WeatherStations(station_id)
);

CREATE TABLE Pressure
(
    pressure_id INT AUTO_INCREMENT PRIMARY KEY,
    station_id VARCHAR(17),
    value DECIMAL(10, 2),
    date DATE,
    time TIME,
    FOREIGN KEY (station_id) REFERENCES WeatherStations(station_id)
);

CREATE TABLE LightIntensity
(
    light_id INT AUTO_INCREMENT PRIMARY KEY,
    station_id VARCHAR(17),
    value DECIMAL(10, 2),
    date DATE,
    time TIME,
    FOREIGN KEY (station_id) REFERENCES WeatherStations(station_id)
);

CREATE TABLE AirQuality
(
    air_quality_id INT AUTO_INCREMENT PRIMARY KEY,
    station_id VARCHAR(17),
    value DECIMAL(10, 2),
    date DATE,
    time TIME,
    FOREIGN KEY (station_id) REFERENCES WeatherStations(station_id)
);

CREATE TABLE RainSensor
(
    rain_id INT AUTO_INCREMENT PRIMARY KEY,
    station_id VARCHAR(17),
    value BOOLEAN,
    date DATE,
    time TIME,
    FOREIGN KEY (station_id) REFERENCES WeatherStations(station_id)
);

CREATE TABLE UVIntensity
(
    uv_id INT AUTO_INCREMENT PRIMARY KEY,
    station_id VARCHAR(17),
    value DECIMAL(5, 2),
    date DATE,
    time TIME,
    FOREIGN KEY (station_id) REFERENCES WeatherStations(station_id)
);

CREATE TABLE SoundLevel
(
    sound_id INT AUTO_INCREMENT PRIMARY KEY,
    station_id VARCHAR(17),
    value DECIMAL(5, 2),
    date DATE,
    time TIME,
    FOREIGN KEY (station_id) REFERENCES WeatherStations(station_id)
);

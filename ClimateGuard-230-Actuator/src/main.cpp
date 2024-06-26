#include <Arduino.h>
#include <EnvironmentConfig.h>
#include <MQTTManager.h>
#include <LEDController.h>
#include <ButtonHandler.h>
#include <ActuatorController.h>
#include <WiFiManager.h>
#include <DisplayManager.h>
#include <AudioController.h>

// Objects for the different components
DisplayManager displayManager;
WiFiClient wifiClient;
MQTTManager mqttManager(wifiClient, displayManager);
LEDController ledController;
ButtonHandler buttonHandler;
ActuatorController actuatorController;
WiFiManager wifiManager(displayManager);
AudioController audioController;

// Variables for the sensor data
float temperature = 0;
float pressure = 0;
float humidity = 0;
bool rainSensor = false;
float lightIntensity = 0;
float airQuality = 0;

// Variables for the button handling
const unsigned long clickInterval = 2000;
const unsigned long debounceDelay = 50;

unsigned long lastDebounceTime = 0;
unsigned long firstClickTime = 0;
int clickCount = 0;
bool lastButtonState = LOW;

unsigned long mqttLastSendTime = 0;
const unsigned long mqttSendInterval = 5000;

const char *mode = Environment::getStationMode();

// functions
void printAll();
void handlePlayPause();

void setup()
{
  Serial.begin(9600);
  delay(2000);

  displayManager.init();
  displayManager.screenOff();
  displayManager.showBootScreen();
  wifiManager.setupWiFi();
  audioController.initialize();

  mqttManager.setup("ESP32_Aktor");
  mqttManager.loop();

  ledController.setupPins();

  actuatorController.setupActuators();

  Serial.println(wifiManager.getMacAddress());

  mqttManager.waitForInitialData();

  char mqttserver[50];
  strncpy(mqttserver, MQTT_SERVER_URL + 7, strlen(MQTT_SERVER_URL) - 13);
  mqttserver[strlen(MQTT_SERVER_URL) - 13] = '\0';
  strcat(mqttserver, ":");
  strcat(mqttserver, String(MQTT_PORT).c_str());
  displayManager.showStartScreen(wifiManager.convertDecimalToIP(mqttManager.getIP()), mqttserver, "A8:42:E3:A9:55:38");
  // displayManager.showStartScreen("IPAdresse", mqttserver, "Macadresse");

  unsigned long startTime = millis();
  while (!buttonHandler.getModusState() && (millis() - startTime) < 15000)
  {
    delay(100);
  }

  displayManager.screenOff();

  mode = Environment::getStationMode();
  mqttManager.publishMode(mode);
}

void loop()
{
  mqttManager.loop();

  temperature = mqttManager.getTemperature();
  pressure = mqttManager.getPressure();
  humidity = mqttManager.getHumidity();
  airQuality = mqttManager.getAirQuality();
  lightIntensity = mqttManager.getLightIntensity();
  rainSensor = mqttManager.getRainSensor();

  Serial.println("Loop gestartet!");
  Serial.println(lightIntensity);

  wifiManager.refreshNTP();

  if (strcmp(mode, "INDOOR") == 0)
  {
    actuatorController.updateServo(lightIntensity);
    actuatorController.updateFan(temperature);
    ledController.updateRGBLed(-300);
    displayManager.updateMeters(mqttManager.getTemperature(), mqttManager.getHumidity(), mqttManager.getPressure(), mqttManager.getLightIntensity());
  }
  else if (strcmp(mode, "OUTDOOR") == 0)
  {
    ledController.updateRGBLed(temperature);
    displayManager.screenOff();
  }
  else
  {
    actuatorController.updateServo(lightIntensity);
    actuatorController.updateFan(temperature);
    ledController.updateRGBLed(temperature);
    displayManager.updateMeters(mqttManager.getTemperature(), mqttManager.getHumidity(), mqttManager.getPressure(), mqttManager.getLightIntensity());
  }

  ledController.updateLED();

  if (buttonHandler.getPlayPauseState())
  {
    Serial.println("Play Pause pressed");
    audioController.speakSensor(temperature, humidity, pressure, false, false);
  }

  if (buttonHandler.getLeiserState())
  {
    audioController.volumeDown();
    while (buttonHandler.getLeiserState())
    {
      delay(100);
    }
  }

  if (buttonHandler.getLauterState())
  {
    audioController.volumeUp();
    while (buttonHandler.getLauterState())
    {
      delay(100);
    }
  }

  mode = Environment::getStationMode();
  if (buttonHandler.getModusState())
  {
    ledController.switchMode();
    mqttManager.publishMode(mode);
    while (buttonHandler.getModusState())
    {
      delay(100);
    }
  }
}

void printAll()
{
  Serial.print("Temperature: ");
  Serial.println(temperature);
  Serial.print("Pressure: ");
  Serial.println(pressure);
  Serial.print("Humidity: ");
  Serial.println(humidity);
  Serial.print("Rain Sensor: ");
  Serial.println(rainSensor ? "Es regnet" : "Es regnet nicht");
  Serial.print("Light Intensity: ");
  Serial.println(lightIntensity);
  Serial.print("Air Quality: ");
  Serial.println(airQuality);
}

void handlePlayPause()
{
  Serial.println("handlePlay");
  buttonHandler.update();
  bool currentButtonState = buttonHandler.getPlayPauseState();

  if (currentButtonState != lastButtonState)
  {
    Serial.println("Button pressed");
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay)
  {
    if (currentButtonState == HIGH && lastButtonState == LOW)
    {
      if (clickCount == 0)
      {
        firstClickTime = millis();
      }
      clickCount++;
    }
    if (currentButtonState == LOW && lastButtonState == HIGH)
    {
      lastDebounceTime = millis();
    }
  }

  if (clickCount > 0 && (millis() - firstClickTime) > clickInterval)
  {
    Serial.println("1");
    if (clickCount == 1)
    {
      audioController.speakTemperature(temperature, false);
    }
    else if (clickCount == 2)
    {
      audioController.speakPressure(pressure, false);
    }
    else if (clickCount == 3)
    {
      audioController.speakHumidity(humidity);
    }
    clickCount = 0;
  }

  lastButtonState = currentButtonState;
}
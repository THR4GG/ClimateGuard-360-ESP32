#include "AudioController.h"

bool AudioController::trackPlaying = false;

AudioController::AudioController()
    : mySoftwareSerial(RX_PIN, TX_PIN), td(mySoftwareSerial, 5) // Initialize SoftwareSerial with RX 16, TX 17, and busy pin 5
{
}

void AudioController::initialize()
{
    mySoftwareSerial.begin(9600);
    td.begin();
    volume = 100;
    td.setVolume(volume);

    // Register callbacks
    td.registerOnDone(tdDone);
    td.registerOnError(tdError);
    td.registerOnCard(tdCard);

    Serial.println("DFPlayer initialization complete.");

    if (td.getStatus() == 512)
    {
        Serial.println("DFPlayer is ready and online.");
    }
    else
    {
        Serial.println("DFPlayer is not ready. Please check connections and SD card.");
    }
}

void AudioController::playTrack(int trackNumber)
{
    waitForTrackToFinish();
    Serial.println("Playing track: " + String(trackNumber));
    td.say(trackNumber);
    trackPlaying = true;
}

void AudioController::setVolume(int newVolume)
{
    volume = constrain(newVolume, 0, 100);
    td.setVolume(volume);
    Serial.println("Volume set to: " + String(volume));
}

void AudioController::volumeUp()
{
    volume = constrain(volume + 5, 0, 100);
    td.setVolume(volume);
    Serial.println("Volume set to: " + String(volume));
}

void AudioController::volumeDown()
{
    volume = constrain(volume - 5, 0, 100);
    td.setVolume(volume);
    Serial.println("Volume set to: " + String(volume));
}

void AudioController::stopTrack()
{
    td.reset();
    trackPlaying = false;
    Serial.println("Track stopped.");
}

void AudioController::waitForTrackToFinish()
{
    while (trackPlaying)
    {
        td.loop();
        delay(100);
    }
}

void AudioController::tdDone(uint16_t track)
{
    Serial.print("Finished playing track ");
    Serial.println(track);
    trackPlaying = false;
}

void AudioController::tdError(String msg)
{
    Serial.println("DFPlayer Error: " + msg);
}

void AudioController::tdCard(uint8_t event)
{
    switch (event)
    {
    case MEDIUM_ONLINE:
        Serial.println("SD-Card online");
        break;
    case MEDIUM_INSERTED:
        Serial.println("SD-Card inserted");
        break;
    case MEDIUM_REMOVED:
        Serial.println("SD-Card removed");
        break;
    }
}

void AudioController::speakSensor(float temperature, float humidity, float pressure, bool isFahrenheit, bool isBar)
{
    waitForTrackToFinish();
    td.say(WORD_TEMPERATURE);
    waitForTrackToFinish();
    td.say(WORD_IS);
    waitForTrackToFinish();

    if (isFahrenheit)
    {
        float temperatureFahrenheit = (temperature * 9 / 5) + 32;
        td.sayFloat(temperatureFahrenheit, 1);
        waitForTrackToFinish();
        td.say(WORD_DEGREE);
        waitForTrackToFinish();
        td.say(WORD_FAHRENHEIT);
        waitForTrackToFinish();
    }
    else
    {
        td.sayFloat(temperature, 1);
        waitForTrackToFinish();
        td.say(WORD_DEGREE);
        waitForTrackToFinish();
        td.say(WORD_CELSIUS);
        waitForTrackToFinish();
    }

    td.say(WORD_AIR_PRESSURE);
    waitForTrackToFinish();
    td.say(WORD_IS);
    waitForTrackToFinish();

    if (isBar)
    {
        float pressureBar = pressure / 1000;
        td.sayFloat(pressureBar, 3);
        waitForTrackToFinish();
        td.say(363);
        waitForTrackToFinish();
    }
    else
    {
        td.sayFloat(pressure, 1);
        waitForTrackToFinish();
        td.say(WORD_HECTOPASCAL);
        waitForTrackToFinish();
    }

    td.say(WORD_HUMIDITY);
    waitForTrackToFinish();
    td.say(WORD_IS);
    waitForTrackToFinish();
    td.sayFloat(humidity, 1);
    waitForTrackToFinish();
    td.say(WORD_PERCENT);
    waitForTrackToFinish();
}

void AudioController::speakTemperature(float temperature, bool isFahrenheit)
{
    waitForTrackToFinish();
    td.say(WORD_TEMPERATURE);
    waitForTrackToFinish();
    td.say(WORD_IS);
    waitForTrackToFinish();

    if (isFahrenheit)
    {
        float temperatureFahrenheit = (temperature * 9 / 5) + 32;
        td.sayFloat(temperatureFahrenheit, 1);
        waitForTrackToFinish();
        td.say(WORD_DEGREE);
        waitForTrackToFinish();
        td.say(WORD_FAHRENHEIT);
        waitForTrackToFinish();
    }
    else
    {
        td.sayFloat(temperature, 1);
        waitForTrackToFinish();
        td.say(WORD_DEGREE);
        waitForTrackToFinish();
        td.say(WORD_CELSIUS);
        waitForTrackToFinish();
    }
}

void AudioController::speakPressure(float pressure, bool isBar)
{
    waitForTrackToFinish();
    td.say(WORD_AIR_PRESSURE);
    waitForTrackToFinish();
    td.say(WORD_IS);
    waitForTrackToFinish();

    if (isBar)
    {
        float pressureBar = pressure / 1000;
        td.sayFloat(pressureBar, 3);
        waitForTrackToFinish();
        td.say(363);
        waitForTrackToFinish();
    }
    else
    {
        td.sayFloat(pressure, 1);
        waitForTrackToFinish();
        td.say(WORD_HECTOPASCAL);
        waitForTrackToFinish();
    }
}

void AudioController::speakHumidity(float humidity)
{
    waitForTrackToFinish();
    td.say(WORD_HUMIDITY);
    waitForTrackToFinish();
    td.say(WORD_IS);
    waitForTrackToFinish();
    td.sayFloat(humidity, 1);
    waitForTrackToFinish();
    td.say(WORD_PERCENT);
    waitForTrackToFinish();
}

void AudioController::say(int file)
{
    waitForTrackToFinish();
    Serial.println("say: " + String(file));
    td.say(file);
    trackPlaying = true;
}

#include <EnvironmentConfig.h>
#include <NTPClient.h>
#include <Talking_Display.h>
#include <SoftwareSerial.h>

#ifndef AUDIOCONTROLLER_H
#define AUDIOCONTROLLER_H

class AudioController
{
public:
    AudioController();
    void initialize();
    void playTrack(int trackNumber);
    void setVolume(int volume);
    void stopTrack();
    void speakSensor(float temperatur, float luftfeuchtigkeit, float luftdruck, bool isFahrenheit, bool isBar);
    void speakTemperature(float temperature, bool isFahrenheit);
    void speakPressure(float pressure, bool isBar);
    void speakHumidity(float humidity);
    void zeitSprechen(NTPClient &timeClient);
    int valueFromString(char *string, int start, int width);
    void volumeUp();
    void volumeDown();
    void say(int file);

private:
    SoftwareSerial mySoftwareSerial;
    Talking_Display<SoftwareSerial> td;
    int volume;
    static bool trackPlaying;
    static void tdDone(uint16_t track);
    static void tdError(String msg);
    static void tdCard(uint8_t event);
    void waitForTrackToFinish();
};

#endif

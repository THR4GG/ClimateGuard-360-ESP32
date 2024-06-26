#include "EnvironmentConfig.h"

// Definition der WiFi Settings
const char *ssids[] = {"Atomlabor", "Raum-25", "iPhone", "Raum-013"};                                 // SSIDs
const char *passwords[] = {"94304555746584134459159942627645", "FESRaum-25", "12345678", "Raum-013"}; // Passwörter
const char *proxies[] = {nullptr, "192.168.10.10:3128", nullptr};                                     // Proxy-Einstellungen
const int networkCount = sizeof(ssids);                                                               // Anzahl der Netzwerke

const char *Environment::stationMode = "INDOOR";

void Environment::setStationMode(const char *mode)
{
    stationMode = mode;
}

// Gibt den aktuellen Station Mode zurück
const char *Environment::getStationMode()
{
    return stationMode;
}
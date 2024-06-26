#include "DisplayManager.h"

DisplayManager::DisplayManager() : tft(TFT_eSPI())
{
}

void DisplayManager::init()
{
    tft.init();
    tft.setRotation(2);
    tft.fillScreen(TFT_BLACK);

    // Berechnung der Bereichsgrößen und Positionen
    areaWidth = tft.width() / 2 - 1;
    areaHeight = (tft.height() - 40) / 2 - 1; // Abzüglich Header und Footer
    radius = 70;
    textHeight = 20;

    // Bereichskoordinaten berechnen
    xpos1 = 1 + areaWidth / 2 - radius;
    ypos1 = 45 + areaHeight / 2 - radius;
    xpos2 = tft.width() / 2 + 1 + areaWidth / 2 - radius;
    ypos2 = ypos1;
    xpos3 = 1 + areaWidth / 2 - radius;
    ypos3 = tft.height() / 2 + 1 + areaHeight / 2 - radius;
    xpos4 = tft.width() / 2 + 1 + areaWidth / 2 - radius;
    ypos4 = ypos3;

    // Bereiche initialisieren
    tft.fillRect(1, 45, areaWidth, areaHeight, TFT_BLACK);
    tft.fillRect(tft.width() / 2 + 1, 45, areaWidth, areaHeight, AREA2_COLOR);
    tft.fillRect(1, tft.height() / 2 + 1, areaWidth, areaHeight, TFT_BLACK);
    tft.fillRect(tft.width() / 2 + 1, tft.height() / 2 + 1, areaWidth, areaHeight, AREA4_COLOR);

    drawHeader();
    drawFooter();
    // drawGrid();
}

void DisplayManager::drawHeader()
{
    int headerHeight = 40;
    tft.fillRect(0, 0, tft.width(), headerHeight, HEADER_COLOR);

    String headerText = "ClimateGuard-360";
    int textSize = 2;
    tft.setTextSize(textSize);
    tft.setTextColor(TFT_WHITE, HEADER_COLOR);

    int16_t textWidth = getTextWidth(headerText, textSize);
    int cursorX = (tft.width() - textWidth) / 2;
    int cursorY = (headerHeight - (8 * textSize)) / 2;

    tft.setCursor(cursorX, cursorY);
    tft.print(headerText);
}

void DisplayManager::drawFooter()
{
    int footerHeight = 40;
    int footerY = tft.height() - footerHeight;

    tft.fillRect(0, footerY, tft.width(), footerHeight, FOOTER_COLOR);

    String footerText = "Magnus Jung, Nico Fellhauer, Joel Paletta";
    int textSize = 1;
    tft.setTextSize(textSize);
    tft.setTextColor(TFT_WHITE, FOOTER_COLOR);

    int16_t textWidth = getTextWidth(footerText, textSize);
    int cursorX = (tft.width() - textWidth) / 2;
    int cursorY = footerY + (footerHeight - (8 * textSize)) / 2;

    tft.setCursor(cursorX, cursorY);
    tft.print(footerText);
}

void DisplayManager::drawGrid()
{
    tft.setTextColor(TFT_WHITE);
    tft.setTextSize(1);

    int centerX = tft.width() / 2;
    int centerY = (40 + (tft.height() - 40)) / 2;

    int thickness = 2;
    for (int i = -thickness / 2; i <= thickness / 2; i++)
    {
        tft.drawLine(centerX + i, 40, centerX + i, tft.height() - 40, TFT_WHITE);
        tft.drawLine(0, centerY + i, tft.width(), centerY + i, TFT_WHITE);
    }
}

void DisplayManager::updateMeters(float temp, float humidity, float pressure, float lightIntensity)
{
    drawHeader();
    drawFooter();

    tft.setTextColor(TFT_WHITE);
    tft.setTextSize(1.5);

    Serial.print("Light Intensity:");
    Serial.println(lightIntensity);

    ringMeter(temp, -20.0, 50.0, xpos1, ypos1, radius, "*C", BLUE2BLUE);
    ringMeter(humidity, 0.0, 100.0, xpos2, ypos2, radius, "%", BLUE2RED);
    ringMeter(pressure, 0.0, 10000.0, xpos3, ypos3, radius, "hPa", RED2GREEN);
    ringMeter(lightIntensity, 0.0, 50.0, xpos4, ypos4, radius, "Lux", BLUE2RED);

    tft.drawCentreString("Temperatur", xpos1 + radius, ypos1 - textHeight - 10, 2);
    tft.drawCentreString("Luftfeuchtigkeit", xpos2 + radius, ypos2 - textHeight - 10, 2);
    tft.drawCentreString("Luftdruck", xpos3 + radius, ypos3 - textHeight - 10, 2);
    tft.drawCentreString("Lichtstaerke", xpos4 + radius, ypos4 - textHeight - 10, 2);
}

int16_t DisplayManager::getTextWidth(String text, int textSize)
{
    tft.setTextSize(textSize);
    return tft.textWidth(text);
}

int DisplayManager::ringMeter(float value, float vmin, float vmax, int x, int y, int r, const char *units, byte scheme)
{
    x += r;
    y += r;

    int w = r / 4;
    float angle = 150.0;
    int text_colour = TFT_WHITE;

    float v = map(value, vmin, vmax, -angle, angle);

    byte seg = 5;
    byte inc = 5;

    for (int i = 0; i < 3; i++)
    {
        tft.drawCircle(x, y, r + i, TFT_WHITE);
    }

    tft.fillCircle(x, y, r - w, INNER_CIRCLE_COLOR);

    for (int i = -angle; i < angle; i += inc)
    {
        int colour = 0;
        switch (scheme)
        {
        case RED2RED:
            colour = TFT_RED;
            break;
        case GREEN2GREEN:
            colour = TFT_GREEN;
            break;
        case BLUE2BLUE:
            colour = TFT_BLUE;
            break;
        case BLUE2RED:
            colour = rainbow(map(i, -angle, angle, 0, 127));
            break;
        case GREEN2RED:
            colour = rainbow(map(i, -angle, angle, 63, 127));
            break;
        case RED2GREEN:
            colour = rainbow(map(i, -angle, angle, 127, 63));
            break;
        default:
            colour = TFT_BLUE;
            break;
        }

        float sx = cos((i - 90) * 0.0174532925);
        float sy = sin((i - 90) * 0.0174532925);
        uint16_t x0 = sx * (r - w) + x;
        uint16_t y0 = sy * (r - w) + y;
        uint16_t x1 = sx * r + x;
        uint16_t y1 = sy * r + y;

        float sx2 = cos((i + seg - 90) * 0.0174532925);
        float sy2 = sin((i + seg - 90) * 0.0174532925);
        int x2 = sx2 * (r - w) + x;
        int y2 = sy2 * (r - w) + y;
        int x3 = sx2 * r + x;
        int y3 = sy2 * r + y;

        if (i < v)
        {
            tft.fillTriangle(x0, y0, x1, y1, x2, y2, colour);
            tft.fillTriangle(x1, y1, x2, y2, x3, y3, colour);
            text_colour = TFT_WHITE;
        }
        else
        {
            tft.fillTriangle(x0, y0, x1, y1, x2, y2, TFT_DARKGREY);
            tft.fillTriangle(x1, y1, x2, y2, x3, y3, TFT_DARKGREY);
        }
    }

    tft.fillCircle(x, y, r - w, INNER_CIRCLE_COLOR);

    char buf[10];
    byte len = 4;
    if (value > 999)
        len = 5;
    dtostrf(value, len, 0, buf);

    tft.setTextColor(TFT_WHITE);
    tft.setTextColor(text_colour);

    if (r > 84)
        tft.drawCentreString(buf, x - 5, y - 20, 6);
    else
        tft.drawCentreString(buf, x - 5, y - 20, 4);

    tft.setTextColor(TFT_WHITE);
    if (r > 84)
        tft.drawCentreString(units, x, y + 30, 4);
    else
        tft.drawCentreString(units, x, y + 5, 2);

    return x + r;
}

unsigned int DisplayManager::rainbow(byte value)
{
    byte red = 0;
    byte green = 0;
    byte blue = 0;

    byte quadrant = value / 32;

    if (quadrant == 0)
    {
        blue = 31;
        green = 2 * (value % 32);
        red = 0;
    }
    if (quadrant == 1)
    {
        blue = 31 - (value % 32);
        green = 63;
        red = 0;
    }
    if (quadrant == 2)
    {
        blue = 0;
        green = 63;
        red = value % 32;
    }
    if (quadrant == 3)
    {
        blue = 0;
        green = 63 - 2 * (value % 32);
        red = 31;
    }

    return (red << 11) + (green << 5) + blue;
}

float DisplayManager::sineWave(int phase)
{
    return sin(phase * 0.02454369);
}

void DisplayManager::screenOff()
{
    tft.fillScreen(TFT_BLACK);
}
void DisplayManager::showWifiError()
{
    int screenWidth = tft.width();
    int screenHeight = tft.height();

    tft.setRotation(2);
    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(TFT_ORANGE);

    tft.setTextSize(3);
    tft.drawCentreString("ClimateGuard-360", screenWidth / 2, screenHeight * 0.02, 1);
    tft.fillRect(0, screenHeight * 0.075, screenWidth, screenHeight * 0.0125, TFT_WHITE);

    float triangleCenterX = screenWidth / 2;
    float triangleHeight = screenHeight * 0.4;
    float triangleBaseWidth = 2 * (triangleHeight / sqrt(3));
    float triangleBaseY = screenHeight * 0.6;

    tft.fillTriangle(triangleCenterX, triangleBaseY - triangleHeight,
                     triangleCenterX - triangleBaseWidth / 2, triangleBaseY,
                     triangleCenterX + triangleBaseWidth / 2, triangleBaseY, TFT_RED);

    float innerTriangleScale = 0.75;
    float innerTriangleHeight = triangleHeight * innerTriangleScale;
    float innerTriangleBaseWidth = 2 * (innerTriangleHeight / sqrt(3));
    float innerTriangleOffsetY = screenHeight * 0.03;

    tft.fillTriangle(triangleCenterX, triangleBaseY - innerTriangleHeight - innerTriangleOffsetY,
                     triangleCenterX - innerTriangleBaseWidth / 2, triangleBaseY - innerTriangleOffsetY,
                     triangleCenterX + innerTriangleBaseWidth / 2, triangleBaseY - innerTriangleOffsetY, TFT_WHITE);

    float exclamationHeight = innerTriangleHeight * 0.4;
    float exclamationWidth = innerTriangleBaseWidth * 0.12;
    tft.fillRoundRect(triangleCenterX - exclamationWidth / 2, triangleBaseY - innerTriangleHeight + (innerTriangleHeight - exclamationHeight) / 2 - innerTriangleOffsetY, exclamationWidth, exclamationHeight, screenWidth * 0.015, TFT_BLACK);

    float circleRadius = screenHeight * 0.02;
    tft.fillCircle(triangleCenterX, triangleBaseY - circleRadius - screenHeight * 0.05, circleRadius, TFT_BLACK);

    tft.setTextSize(3);
    tft.setTextColor(TFT_RED);
    tft.drawCentreString("WiFi-Error", screenWidth / 2, triangleBaseY + screenHeight * 0.07, 1);

    tft.setTextSize(1);
    tft.setTextColor(TFT_WHITE);
    tft.drawCentreString("verbinde...", screenWidth / 2, screenHeight * 0.85, 1);
}

void DisplayManager::showMqttError()
{
    int screenWidth = tft.width();
    int screenHeight = tft.height();

    tft.setRotation(2);
    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(TFT_ORANGE);

    tft.setTextSize(3);
    tft.drawCentreString("ClimateGuard-360", screenWidth / 2, screenHeight * 0.02, 1);
    tft.fillRect(0, screenHeight * 0.075, screenWidth, screenHeight * 0.0125, TFT_WHITE);

    float triangleCenterX = screenWidth / 2;
    float triangleHeight = screenHeight * 0.4;
    float triangleBaseWidth = 2 * (triangleHeight / sqrt(3));
    float triangleBaseY = screenHeight * 0.6;

    tft.fillTriangle(triangleCenterX, triangleBaseY - triangleHeight,
                     triangleCenterX - triangleBaseWidth / 2, triangleBaseY,
                     triangleCenterX + triangleBaseWidth / 2, triangleBaseY, TFT_RED);

    float innerTriangleScale = 0.75;
    float innerTriangleHeight = triangleHeight * innerTriangleScale;
    float innerTriangleBaseWidth = 2 * (innerTriangleHeight / sqrt(3));
    float innerTriangleOffsetY = screenHeight * 0.03;

    tft.fillTriangle(triangleCenterX, triangleBaseY - innerTriangleHeight - innerTriangleOffsetY,
                     triangleCenterX - innerTriangleBaseWidth / 2, triangleBaseY - innerTriangleOffsetY,
                     triangleCenterX + innerTriangleBaseWidth / 2, triangleBaseY - innerTriangleOffsetY, TFT_WHITE);

    float exclamationHeight = innerTriangleHeight * 0.4;
    float exclamationWidth = innerTriangleBaseWidth * 0.12;
    tft.fillRoundRect(triangleCenterX - exclamationWidth / 2, triangleBaseY - innerTriangleHeight + (innerTriangleHeight - exclamationHeight) / 2 - innerTriangleOffsetY, exclamationWidth, exclamationHeight, screenWidth * 0.015, TFT_BLACK);

    float circleRadius = screenHeight * 0.02;
    tft.fillCircle(triangleCenterX, triangleBaseY - circleRadius - screenHeight * 0.05, circleRadius, TFT_BLACK);

    tft.setTextSize(3);
    tft.setTextColor(TFT_RED);
    tft.drawCentreString("MQTT-Error", screenWidth / 2, triangleBaseY + screenHeight * 0.07, 1);

    tft.setTextSize(1);
    tft.setTextColor(TFT_WHITE);
    tft.drawCentreString("verbinde...", screenWidth / 2, screenHeight * 0.85, 1);
}

void DisplayManager::showError(String errorType)
{
    int screenWidth = tft.width();
    int screenHeight = tft.height();

    tft.setRotation(2);
    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(TFT_ORANGE);

    tft.setTextSize(3);
    tft.drawCentreString("ClimateGuard-360", screenWidth / 2, screenHeight * 0.02, 1);
    tft.fillRect(0, screenHeight * 0.075, screenWidth, screenHeight * 0.0125, TFT_WHITE);

    float triangleCenterX = screenWidth / 2;
    float triangleHeight = screenHeight * 0.4;
    float triangleBaseWidth = 2 * (triangleHeight / sqrt(3));
    float triangleBaseY = screenHeight * 0.6;

    tft.fillTriangle(triangleCenterX, triangleBaseY - triangleHeight,
                     triangleCenterX - triangleBaseWidth / 2, triangleBaseY,
                     triangleCenterX + triangleBaseWidth / 2, triangleBaseY, TFT_RED);

    float innerTriangleScale = 0.75;
    float innerTriangleHeight = triangleHeight * innerTriangleScale;
    float innerTriangleBaseWidth = 2 * (innerTriangleHeight / sqrt(3));
    float innerTriangleOffsetY = screenHeight * 0.03;

    tft.fillTriangle(triangleCenterX, triangleBaseY - innerTriangleHeight - innerTriangleOffsetY,
                     triangleCenterX - innerTriangleBaseWidth / 2, triangleBaseY - innerTriangleOffsetY,
                     triangleCenterX + innerTriangleBaseWidth / 2, triangleBaseY - innerTriangleOffsetY, TFT_WHITE);

    float exclamationHeight = innerTriangleHeight * 0.4;
    float exclamationWidth = innerTriangleBaseWidth * 0.12;
    tft.fillRoundRect(triangleCenterX - exclamationWidth / 2, triangleBaseY - innerTriangleHeight + (innerTriangleHeight - exclamationHeight) / 2 - innerTriangleOffsetY, exclamationWidth, exclamationHeight, screenWidth * 0.015, TFT_BLACK);

    float circleRadius = screenHeight * 0.02;
    tft.fillCircle(triangleCenterX, triangleBaseY - circleRadius - screenHeight * 0.05, circleRadius, TFT_BLACK);

    tft.setTextSize(3);
    tft.setTextColor(TFT_RED);
    tft.drawCentreString(errorType, screenWidth / 2, triangleBaseY + screenHeight * 0.07, 1);
}

void DisplayManager::showStartScreen(String ownIP, String mqttServer, String macAddress)
{
    int screenWidth = tft.width();
    int screenHeight = tft.height();

    tft.setRotation(2);
    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(TFT_ORANGE);

    tft.setTextSize(3);
    tft.drawCentreString("ClimateGuard-360", screenWidth / 2, screenHeight * 0.02, 1);
    tft.fillRect(0, screenHeight * 0.075, screenWidth, screenHeight * 0.0125, TFT_WHITE);

    float rectX = screenWidth * 0.0417;
    float rectWidth = screenWidth * 0.9167;
    float rectHeight = screenHeight * 0.12;
    float cornerRadius = screenWidth * 0.0208;

    float ipRectY = screenHeight * 0.18;
    tft.setTextSize(2);
    tft.setTextColor(TFT_WHITE);
    tft.drawCentreString("IP-Adresse", screenWidth / 2, ipRectY - screenHeight * 0.06, 1); // Oberhalb des Rechtecks
    tft.setTextColor(TFT_GREEN);
    tft.drawCentreString(ownIP, screenWidth / 2, ipRectY + (rectHeight / 2) - (tft.fontHeight(2) / 2), 2); // Vertikal zentriert im Rechteck
    tft.drawRoundRect(rectX, ipRectY, rectWidth, rectHeight, cornerRadius, TFT_BLUE);

    float macRectY = screenHeight * 0.38;
    tft.setTextColor(TFT_WHITE);
    tft.drawCentreString("MAC-Adresse", screenWidth / 2, macRectY - screenHeight * 0.06, 1); // Oberhalb des Rechtecks
    tft.setTextColor(TFT_GREEN);
    tft.drawCentreString(macAddress, screenWidth / 2, macRectY + (rectHeight / 2) - (tft.fontHeight(2) / 2), 2); // Vertikal zentriert im Rechteck
    tft.drawRoundRect(rectX, macRectY, rectWidth, rectHeight, cornerRadius, TFT_BLUE);

    float mqttRectY = screenHeight * 0.58;
    tft.setTextColor(TFT_WHITE);
    tft.drawCentreString("MQTT-Server", screenWidth / 2, mqttRectY - screenHeight * 0.06, 1); // Oberhalb des Rechtecks
    tft.setTextColor(TFT_GREEN);
    tft.drawCentreString(mqttServer, screenWidth / 2, mqttRectY + (rectHeight / 2) - (tft.fontHeight(2) / 2), 2); // Vertikal zentriert im Rechteck
    tft.drawRoundRect(rectX, mqttRectY, rectWidth, rectHeight, cornerRadius, TFT_BLUE);

    tft.setTextColor(TFT_RED);
    tft.setTextSize(1);
    tft.drawCentreString("2024 - Made by: Magnus Jung", screenWidth / 2, screenHeight * 0.95, 1);
}

void DisplayManager::showBootScreen()
{
    int screenWidth = tft.width();
    int screenHeight = tft.height();

    tft.setRotation(2);
    tft.fillScreen(TFT_BLACK);

    // Farben definieren
    uint16_t skinColor = TFT_WHITE; // Placeholder for actual skin color if available
    uint16_t hatColor = TFT_NAVY;
    uint16_t uniformColor = TFT_BLUE;
    uint16_t badgeColor = TFT_WHITE;

    // Figur - Kopf, Hut, Schultern, Arme und Oberkörper
    int centerX = screenWidth / 2;
    int centerY = screenHeight * 0.4;
    int figureWidth = screenWidth * 0.4;
    int figureHeight = screenHeight * 0.6;

    // Kopf
    int headRadius = figureHeight * 0.12;
    int headCenterY = centerY - figureHeight * 0.2;
    tft.fillCircle(centerX, headCenterY, headRadius, skinColor);

    // Hut
    int hatWidth = headRadius * 2;
    int hatHeight = headRadius * 0.5;
    tft.fillRect(centerX - hatWidth / 2, headCenterY - headRadius - hatHeight / 2, hatWidth, hatHeight, hatColor); // Hutbasis
    tft.fillTriangle(centerX - hatWidth / 2, headCenterY - headRadius - hatHeight / 2,
                     centerX + hatWidth / 2, headCenterY - headRadius - hatHeight / 2,
                     centerX, headCenterY - headRadius - hatHeight, hatColor); // Hutspitze

    // Gesicht (Augen und Mund)
    int eyeRadius = headRadius * 0.1;
    tft.fillCircle(centerX - headRadius * 0.3, headCenterY - headRadius * 0.2, eyeRadius, TFT_BLACK); // Linkes Auge
    tft.fillCircle(centerX + headRadius * 0.3, headCenterY - headRadius * 0.2, eyeRadius, TFT_BLACK); // Rechtes Auge

    // Mund als Linienbogen simulieren
    int mouthStartAngle = 200;
    int mouthEndAngle = 340;
    int mouthRadius = headRadius * 0.6;
    for (int angle = mouthStartAngle; angle <= mouthEndAngle; angle += 10)
    {
        float radian = angle * DEG_TO_RAD;
        int x = centerX + mouthRadius * cos(radian);
        int y = headCenterY + mouthRadius * sin(radian);
        tft.drawPixel(x, y, TFT_BLACK);
    }

    // Schultern
    int shoulderWidth = figureWidth * 0.8;
    int shoulderHeight = figureHeight * 0.1;
    int shoulderY = headCenterY + headRadius;
    tft.fillRect(centerX - shoulderWidth / 2, shoulderY, shoulderWidth, shoulderHeight, uniformColor);

    // Arme
    int armWidth = figureWidth * 0.12;
    int armHeight = figureHeight * 0.25;
    int armY = shoulderY + shoulderHeight;
    tft.fillRect(centerX - figureWidth / 2 - armWidth * 0.1, armY, armWidth, armHeight, uniformColor); // Linker Arm
    tft.fillRect(centerX + figureWidth / 2 - armWidth * 0.9, armY, armWidth, armHeight, uniformColor); // Rechter Arm

    // Oberkörper (kürzer und kantiger)
    int bodyWidth = figureWidth * 0.6;
    int bodyHeight = figureHeight * 0.3;
    int bodyY = shoulderY + shoulderHeight;
    tft.fillRect(centerX - bodyWidth / 2, bodyY, bodyWidth, bodyHeight, uniformColor);

    // Kragen (kürzer und kantiger)
    int collarWidth = bodyWidth * 0.8;
    int collarHeight = bodyHeight * 0.2;
    tft.fillRect(centerX - collarWidth / 2, shoulderY, collarWidth, collarHeight, badgeColor);

    // Text "CG" auf der Brust
    tft.setTextColor(TFT_WHITE);
    tft.setTextSize(2);
    tft.drawCentreString("CG", centerX, bodyY + bodyHeight * 0.3, 2);

    // Text "ClimateGuard"
    tft.setTextSize(3);
    tft.drawCentreString("ClimateGuard", screenWidth / 2, screenHeight * 0.75, 1);

    // Text "Est. 2024"
    tft.setTextSize(2);
    tft.drawCentreString("Est. 2024", screenWidth / 2, screenHeight * 0.85, 1);

    // Text "Founded by Magnus Jung"
    tft.setTextSize(2);
    tft.drawCentreString("Founded by Magnus Jung", screenWidth / 2, screenHeight * 0.9, 1);

    // Footer-Text für Boot-Info
    tft.setTextSize(1);
    tft.drawCentreString("Configuring system, please wait...", screenWidth / 2, screenHeight * 0.95, 1);
}

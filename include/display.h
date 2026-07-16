#pragma once
#include <cmath>
#include <TFT_eSPI.h>
#include <time.h>

constexpr uint8_t HOME_X = 190;
constexpr uint8_t HOME_Y = 210;


class Display
{
public:
    void drawWiFiSymbol(int x, int y, uint16_t color = TFT_WHITE);
    void drawHomeSymbol(int x, int y, uint16_t color = TFT_WHITE);
    void drawRefreshSymbol(int x, int y, uint16_t color = TFT_WHITE);
    void drawClockSymbol(int x, int y, uint16_t color = TFT_WHITE);
    void drawFitnessSymbol(int x, int y, uint16_t color = TFT_WHITE);
    void drawBatterySymbol(int x, int y, int battery, uint16_t color = TFT_WHITE);

    void drawSettingsSymbol(int x, int y, uint16_t color = TFT_WHITE);
    void drawTime(struct tm *timeInfo);
    void drawDate(struct tm *timeInfo);
    void drawStepCount(uint32_t steps);

    // Basic Shapes
    void drawRectangle(int x, int y, int width, int height, uint32_t color = TFT_WHITE);
    void clear_screen();

    // App Screens
    void drawStopWatchStart();
    void drawFitnessStart();
    void drawSettingsStart();

    Display() : font_(1), size_(239){};
    Display(uint8_t font) : font_(font) {};
    void init();

    void set_font(uint8_t font);

    /**
     * TODO
     * WRITE YOUR PUBLIC MEMBERS AND FUNCTION HEADERS HERE
     */
    
    void print_d(const char *str, int x, int y, uint8_t font_size = 2);
private:
    TFT_eSPI tft;
    /**
     * TODO
     * WRITE YOUR PRIVATE MEMBERS AND FUNCTIONS HEADERS HERE
     */

    
    uint8_t font_;
    uint8_t size_; // both width and height
    
};
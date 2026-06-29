#include "display.h"

/**
 * TODO
 * WRITE YOUR CLASS FUNCTION IMPLEMENTATIONS HERE
 */
void Display::init(){
    tft.begin();
    tft.setRotation(2);
    tft.fillScreen(TFT_BLACK);
}



void Display::print_d(const char *str, int x, int y, uint8_t font_size){
    
    tft.setTextSize(font_size);
    tft.setTextWrap(true, true);
    tft.setTextColor(TFT_GREEN, TFT_BLACK);
    tft.drawString(str, x, y, font_);
    
}

void Display::set_font(uint8_t font){
    font_ = font;
}

 
void Display::drawSettingsSymbol(int x, int y, uint16_t color)
{
    int r1 = 8;
    int r2 = 16;
    int r3 = 24;
    tft.fillCircle(x, y, 2, color);
    tft.drawArc(x, y, r1, r1 - 2, 135, 225, color, TFT_BLACK);
    tft.drawArc(x, y, r2, r2 - 2, 135, 225, color, TFT_BLACK);
    tft.drawArc(x, y, r3, r3 - 2, 135, 225, color, TFT_BLACK);
}

void Display::drawWiFiSymbol(int x, int y, uint16_t color)
{
    int r1 = 8;
    int r2 = 16;
    int r3 = 24;
    tft.fillCircle(x, y, 2, color);
    tft.drawArc(x, y, r1, r1 - 2, 135, 225, color, TFT_BLACK);
    tft.drawArc(x, y, r2, r2 - 2, 135, 225, color, TFT_BLACK);
    tft.drawArc(x, y, r3, r3 - 2, 135, 225, color, TFT_BLACK);
}

void Display::drawHomeSymbol(int x, int y, uint16_t color)
{
    int width = 40;
    int height = 35;
    int roofHeight = 15;
    int x0 = x;
    int x1 = x + width / 2;
    int x2 = x + width;
    int y0 = y;
    int y1 = y - roofHeight;
    tft.fillTriangle(x0, y0, x1, y1, x2, y0, color);
    int bodyHeight = height - roofHeight;
    int bodyY = y;
    tft.fillRect(x0 + 8, bodyY, width - 16, bodyHeight, color);
    int doorW = 8;
    int doorH = 10;
    int doorX = x + (width / 2) - (doorW / 2);
    int doorY = y + bodyHeight - doorH;
    tft.fillRect(doorX, doorY, doorW, doorH, TFT_BLACK);
    tft.drawTriangle(x0, y0, x1, y1, x2, y0, TFT_WHITE);
    tft.drawRect(x0 + 8, bodyY, width - 16, bodyHeight, TFT_WHITE);
}

void Display::drawRefreshSymbol(int x, int y, uint16_t color)
{
    int radius = 18;
    int thickness = 3;
    int start_angle = 45;
    int end_angle = 315;
    tft.drawArc(x, y, radius, radius - thickness, start_angle, end_angle, color, TFT_BLACK);
    float arrow_angle = (end_angle + 8) * 0.0174533;
    int arrow_len = 8;
    int offset_y = 4;
    int tip_x = x + radius * cos(arrow_angle);
    int tip_y = y - radius * sin(arrow_angle) + offset_y;
    int base1_x = tip_x - arrow_len * cos(arrow_angle - 0.5);
    int base1_y = tip_y + arrow_len * sin(arrow_angle - 0.5);
    int base2_x = tip_x - arrow_len * cos(arrow_angle + 0.5);
    int base2_y = tip_y + arrow_len * sin(arrow_angle + 0.5);
    tft.fillTriangle(tip_x, tip_y, base1_x, base1_y, base2_x, base2_y, color);
}

void Display::drawClockSymbol(int x, int y, uint16_t color)
{
    int r = 16;
    tft.drawCircle(x, y, r, color);
    tft.fillCircle(x, y, 2, color);
    int hour_len = r - 6;
    int hour_x = x - hour_len * cos(30 * 0.0174533);
    int hour_y = y - hour_len * sin(30 * 0.0174533);
    tft.drawLine(x, y, hour_x, hour_y, color);
    int min_len = r - 3;
    int min_x = x + min_len * cos(60 * 0.0174533);
    int min_y = y - min_len * sin(60 * 0.0174533);
    tft.drawLine(x, y, min_x, min_y, color);
}

void Display::drawFitnessSymbol(int x, int y, uint16_t color)
{
    int box_w = 40;
    int box_h = 24;
    int corner_r = 4;
    tft.drawRoundRect(x - box_w / 2, y - box_h / 2, box_w, box_h, corner_r, color);
    int w = box_w - 8;
    int h = box_h / 2;
    int cx = x;
    int cy = y;
    tft.drawLine(cx - w / 2, cy, cx - w / 4, cy, color);
    tft.drawLine(cx - w / 4, cy, cx - w / 8, cy - h / 2, color);
    tft.drawLine(cx - w / 8, cy - h / 2, cx, cy + h / 2, color);
    tft.drawLine(cx, cy + h / 2, cx + w / 8, cy - h / 3, color);
    tft.drawLine(cx + w / 8, cy - h / 3, cx + w / 2, cy, color);
}

void Display::drawBatterySymbol(int x, int y, int battery, uint16_t color)
{
    int box_w = 50;
    int box_h = 24;
    int corner_r = 4;
    int terminal_w = 4;
    tft.drawRoundRect(x - box_w / 2, y - box_h / 2, box_w, box_h, corner_r, color);
    int term_x = x + box_w / 2;
    int term_y = y - box_h / 4;
    tft.drawRect(term_x, term_y, terminal_w, box_h / 2, color);
    int fill_margin = 4;
    if (battery > 100)
    {
        battery = 100;
    }
    int fill_width = (box_w - 2 * fill_margin) * ((float)battery / 100);
    int fill_height = box_h - 2 * fill_margin;
    int fill_x = x - box_w / 2 + fill_margin;
    int fill_y = y - fill_height / 2;
    tft.fillRect(fill_x, fill_y, fill_width, fill_height, TFT_GREEN);
    String percent = String(battery) + "%";
}

void Display::drawTime(struct tm *timeInfo){
    char str[8];
    strftime(str,sizeof(str),"%H:%M",timeInfo);
    print_d(str, 65, 160, 4);
}
void Display::drawDate(struct tm *timeInfo){
    char str[11];
    strftime(str,sizeof(str),"%m/%d/%Y",timeInfo);
    print_d(str, 65, 200, 2);
}

void Display::drawStepCount(uint32_t steps){
    print_d("Step Count:", 45, 120);
    char buf[12];
    snprintf(buf, sizeof(buf), "%d", steps);
    print_d(buf, 175, 120);
}

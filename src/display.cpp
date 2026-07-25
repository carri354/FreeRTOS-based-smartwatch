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



void Display::print_d(const char *str, int x, int y, uint8_t font_size, uint16_t fg_color, uint16_t bg_color){
    
    tft.setTextSize(font_size);
    tft.setTextWrap(true, true);
    tft.setTextColor(fg_color, bg_color);
    tft.drawString(str, x, y, font_);
    
}

void Display::set_font(uint8_t font){
    font_ = font;
}

 
void Display::drawSettingsSymbol(int x, int y, uint16_t color)
{
    int radius = 15;
    int teeth = 6;
    int innerR = radius * 0.65;   // root circle (base of teeth)
    int toothLen = radius * 0.35; // how far teeth stick out past innerR
    int toothHalfW = radius * 0.22;
    int holeR = radius * 0.3;     // center hole

    // base disc
    tft.fillCircle(x, y, innerR, color);

    // teeth
    for (int i = 0; i < teeth; i++) {
        float angle = i * (360.0 / teeth) * DEG_TO_RAD;
        float perp = angle + PI / 2;

        float baseX = x + cos(angle) * innerR;
        float baseY = y + sin(angle) * innerR;
        float tipX  = x + cos(angle) * (innerR + toothLen);
        float tipY  = y + sin(angle) * (innerR + toothLen);

        float dx = cos(perp) * toothHalfW;
        float dy = sin(perp) * toothHalfW;

        int x1 = baseX + dx, y1 = baseY + dy;
        int x2 = baseX - dx, y2 = baseY - dy;
        int x3 = tipX + dx * 0.5f, y3 = tipY + dy * 0.5f;
        int x4 = tipX - dx * 0.5f, y4 = tipY - dy * 0.5f;

        tft.fillTriangle(x1, y1, x2, y2, x3, y3, color);
        tft.fillTriangle(x2, y2, x3, y3, x4, y4, color);
    }

    // punch out center hole with background color
    tft.fillCircle(x, y, holeR, TFT_BLACK);
}
void Display::drawBluetoothSymbol(int x, int y, uint16_t color, int size)
{
    int thickness = 2;
    int h = size;         // half-height
    int w = size * 0.7;   // apex horizontal offset from spine
    int v = size * 0.42;  // apex vertical offset from center

    int topX = x,     topY = y - h;
    int botX = x,     botY = y + h;
    int rUpX = x + w, rUpY = y - v;
    int rDnX = x + w, rDnY = y + v;
    int lUpX = x - w, lUpY = y - v;
    int lDnX = x - w, lDnY = y + v;

    tft.drawWideLine(topX, topY, botX, botY, thickness, color); // spine
    tft.drawWideLine(topX, topY, rUpX, rUpY, thickness, color); // upper flag outer edge
    tft.drawWideLine(botX, botY, rDnX, rDnY, thickness, color); // lower flag outer edge
    tft.drawWideLine(rUpX, rUpY, lDnX, lDnY, thickness, color); // diagonal: upper-right -> lower-left
    tft.drawWideLine(rDnX, rDnY, lUpX, lUpY, thickness, color); // diagonal: lower-right -> upper-left
}

void Display::drawWiFiSymbol(int x, int y, uint16_t full_color, uint16_t empty_color, int8_t signal_strength)
{
    if (signal_strength > 0) {
        signal_strength = 0;
    }
    if (signal_strength < -100) {
        signal_strength = -100;
    }

    int r1 = 6;
    int r2 = 12;
    int r3 = 18;

    uint16_t color1 = signal_strength < 0 && signal_strength > -80 ? full_color : empty_color; // any usable signal
    uint16_t color2 = signal_strength < 0 && signal_strength > -70 ? full_color : empty_color; // fair
    uint16_t color3 = signal_strength < 0 && signal_strength > -67 ? full_color : empty_color; // good
    uint16_t color4 = signal_strength < 0 && signal_strength > -55 ? full_color : empty_color; // excellent

    tft.fillCircle(x, y, 2, color1);
    tft.drawArc(x, y, r1, r1 - 2, 135, 225, color2, TFT_BLACK);
    tft.drawArc(x, y, r2, r2 - 2, 135, 225, color3, TFT_BLACK);
    tft.drawArc(x, y, r3, r3 - 2, 135, 225, color4, TFT_BLACK);
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
    int box_w = 25;
    int box_h = 16;
    int corner_r = 3;
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

    color = battery > 15 ? TFT_GREEN : TFT_RED;
    tft.fillRect(fill_x, fill_y, fill_width, fill_height, color);
    

    
    char percent[4];
    snprintf(percent, sizeof(percent), "%d%%", battery);

    int text_w = strlen(percent) * 6;  // ~6px per char at size 1
    int px = x - text_w / 2;
    int py = y + box_h / 2 + 4;        // 4px gap below the box

    print_d(percent, px, py, 1, TFT_WHITE);
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
    char buf[12];
    snprintf(buf, sizeof(buf), "%d", steps);
    print_d(buf, 110, 140, 3);
}


void Display::drawRectangle(int x, int y, int width, int height, uint32_t color){
    tft.fillRect(x,y,width,height,color);
}


void Display::drawStopWatchStart(){
    clear_screen();
    drawHomeSymbol(HOME_X, HOME_Y);
    print_d("00:00:00", 20, 80, 4);

    drawRectangle(50,120, 65, 20, TFT_DARKGREY);
    drawRectangle(140,120, 55, 20, TFT_DARKGREY);
    drawRectangle(100, 150, 65, 20, TFT_DARKGREY);

    print_d("Start", 53, 123, 2, TFT_GREEN, TFT_DARKGREY);
    print_d("Stop", 143, 123, 2, TFT_GREEN, TFT_DARKGREY);
    print_d("Reset", 103, 153, 2, TFT_GREEN, TFT_DARKGREY);
}

void Display::drawSettingsStart(){
    clear_screen();
    drawHomeSymbol(HOME_X, HOME_Y);

    print_d("Wifi", 20, WIFI_Y);
    print_d("Bluetooth", 20, BT_Y);
}

void Display::drawFitnessStart(){
    clear_screen();
    drawHomeSymbol(HOME_X, HOME_Y);
    print_d("Step Count", 30, 100, 3);
}

void Display::clear_screen(){
    tft.fillRect(0,32,240,205, TFT_BLACK);
}

void Display::reset_screen(){
    tft.fillScreen(TFT_BLACK);
}
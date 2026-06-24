#pragma once
#include <AXP20X.h>
#include <stdint.h>

constexpr uint8_t AXP_INT_PIN  = 35;


void IRAM_ATTR AXP202_BTN_Callback();

class Power
{
    public: 
    /**
    * TODO
    * WRITE YOUR PUBLIC MEMBERS AND FUNCTION HEADERS HERE
    */
   Power();
   


   void init();
   void power_on();
   void power_off();
   void toggle_power();
   void reset_irq();

   int power_display();
    private:
    /**
    * TODO
    * WRITE YOUR PRIVATE MEMBERS AND FUNCTIONS HEADERS HERE
    */

   


    AXP20X_Class axp;
    uint8_t i2c_sda;
    uint8_t i2c_scl;
    bool is_on;
};
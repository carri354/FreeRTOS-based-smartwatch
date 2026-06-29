#include "power.h"

/**
 * TODO
 * WRITE YOUR CLASS FUNCTION IMPLEMENTATIONS HERE
 */

// NON-CLASS


volatile bool power_button_flag = false; // 0 = off, 1 = on

void IRAM_ATTR AXP202_BTN_Callback(){
    power_button_flag = true;
}


// POWER CLASS

Power::Power(){
    i2c_sda = 21;
    i2c_scl = 22;
    
}

void Power::init(){
    power_on();
    axp.enableIRQ(AXP202_PEK_SHORTPRESS_IRQ | AXP202_PEK_LONGPRESS_IRQ, true);
    reset_irq();
}

void Power::reset_irq(){
    axp.readIRQ();
    axp.clearIRQ();
}

void Power::toggle_power(){
    is_on ? power_off() : power_on();
}

void Power::power_on(){
    int ret;
    
    

    ret = axp.begin(Wire);

    if (ret == AXP_FAIL) {
        Serial.println("AXP Power begin failed");
        while (1);
    }

    ret = power_display();

    if (ret == 0) {
        Serial.println("AXP Display power failed");
        while (1);
    }

    is_on = true;

    

}

void Power::power_off(){
    int ret;

    axp.setPowerOutPut(AXP202_LDO2, false);


    is_on = false;
}

int Power::power_display(){
    axp.setPowerOutPut(AXP202_LDO2, true);
    return axp.isLDO2Enable();
}

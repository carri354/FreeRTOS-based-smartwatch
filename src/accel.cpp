#include "accel.h"

/**
 * TODO
 * WRITE YOUR CLASS FUNCTION IMPLEMENTATIONS HERE
 */


SemaphoreHandle_t accel_sem;

void IRAM_ATTR BMA423_Callback(){
   BaseType_t xHigherPriorityTaskWoken = pdFALSE;
  xSemaphoreGiveFromISR(accel_sem, &xHigherPriorityTaskWoken);
  portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

 Accel::Accel(){
    i2c_sda = 21;
    i2c_scl = 22;
    i2c_addr = 0x19;
 }

 void Accel::init(){
   
    if (bma.begin(Wire, i2c_addr, i2c_sda, i2c_scl) == false) {
        handle_error("BMA423 was not found. Please check wiring/power. ");
    }
     
    // Configure the BMA423 accelerometer
    bma.configAccelerometer(
      SensorBMA423::RANGE_4G, 
      SensorBMA423::ODR_100HZ,
      SensorBMA423::BW_NORMAL_AVG4,
      SensorBMA423::PERF_CONTINUOUS_MODE
   );
   
   if (bma.enableAccelerometer() == false){
      handle_error("BMA accelerometer not enabled");
   }

   bma.configInterrupt();

   // Step counter
   bma.resetPedometer();

   bma.enableFeature(SensorBMA423::FEATURE_STEP_CNTR, true);
   bma.enablePedometer();
   bma.enablePedometerIRQ();
   

   

   
   
 }

 void Accel::reset_irq(){
   bma.readIrqStatus();
 }

 uint32_t Accel::get_step_count(){
   return bma.getPedometerCounter();
 }

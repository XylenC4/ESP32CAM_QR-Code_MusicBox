/***************************************************************************
  Example sketch for the INA226_WE library

  This sketch shows how to use the INA226 module in continuous mode.

  Further information can be found on:
  https://wolles-elektronikkiste.de/ina226 (German)
  https://wolles-elektronikkiste.de/en/ina226-current-and-power-sensor (English)

***************************************************************************/
#include <Wire.h>
#include <INA226_WE.h>
#define I2C_ADDRESS 0x40
#define I2C_SDA 15
#define I2C_SCL 14

extern float shuntVoltage_mV = 0.0;
extern float loadVoltage_V = 0.0;
extern float busVoltage_V = 0.0;
extern float current_mA = 0.0;
extern float power_mW = 0.0;

INA226_WE ina226(I2C_ADDRESS);
// INA226_WE ina226 = INA226_WE(); // Alternative: sets default address 0x40

void setup_INA226() {
  TelnetStream.println("INA226: Initialisation");
  Wire.begin(I2C_SDA, I2C_SCL);
  ina226.init();

  /* Set Number of measurements for shunt and bus voltage which shall be averaged
    Mode *     * Number of samples
    AVERAGE_1            1 (default)
    AVERAGE_4            4
    AVERAGE_16          16
    AVERAGE_64          64
    AVERAGE_128        128
    AVERAGE_256        256
    AVERAGE_512        512
    AVERAGE_1024      1024
  */
  //ina226.setAverage(AVERAGE_16); // choose mode and uncomment for change of default

  /* Set conversion time in microseconds
     One set of shunt and bus voltage conversion will take:
     number of samples to be averaged x conversion time x 2

       Mode *         * conversion time
     CONV_TIME_140          140 µs
     CONV_TIME_204          204 µs
     CONV_TIME_332          332 µs
     CONV_TIME_588          588 µs
     CONV_TIME_1100         1.1 ms (default)
     CONV_TIME_2116       2.116 ms
     CONV_TIME_4156       4.156 ms
     CONV_TIME_8244       8.244 ms
  */
  //ina226.setConversionTime(CONV_TIME_1100); //choose conversion time and uncomment for change of default

  /* Set measure mode
    POWER_DOWN - INA226 switched off
    TRIGGERED  - measurement on demand
    CONTINUOUS  - continuous measurements (default)
  */
  //ina226.setMeasureMode(CONTINUOUS); // choose mode and uncomment for change of default

  /* Set Current Range
      Mode *   * Max Current
     MA_400          400 mA
     MA_800          800 mA (default)
  */
  //ina226.setCurrentRange(MA_800); // choose gain and uncomment for change of default

  /* If the current values delivered by the INA226 differ by a constant factor
     from values obtained with calibrated equipment you can define a correction factor.
     Correction factor = current delivered from calibrated equipment / current delivered by INA226
  */
  ina226.setCorrectionFactor(10); //0.01 Ohm shunt

  TelnetStream.println("INA226:  Current Sensor Example Sketch - Continuous");

  ina226.waitUntilConversionCompleted(); //if you comment this line the first data might be zero
}

void loop_INA226() {

  const long readInterval = 20000; // interval at which to blink (milliseconds)
  static unsigned long previousMillis = 0;        // will store last time LED was updated
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= readInterval) {
    // save the last time you blinked the LED
    previousMillis = currentMillis;

    ina226.readAndClearFlags();
    shuntVoltage_mV = ina226.getShuntVoltage_mV();
    busVoltage_V = ina226.getBusVoltage_V();
    current_mA = ina226.getCurrent_mA();
    power_mW = ina226.getBusPower();
    loadVoltage_V  = busVoltage_V + (shuntVoltage_mV / 1000);

    TelnetStream.print("INA226: Shunt Voltage [mV]: "); TelnetStream.println(shuntVoltage_mV);
    TelnetStream.print("INA226: Bus Voltage [V]: "); TelnetStream.println(busVoltage_V);
    TelnetStream.print("INA226: Load Voltage [V]: "); TelnetStream.println(loadVoltage_V);
    TelnetStream.print("INA226: Current[mA]: "); TelnetStream.println(current_mA);
    TelnetStream.print("INA226: Bus Power [mW]: "); TelnetStream.println(power_mW);
    if (!ina226.overflow) {
      TelnetStream.println("INA226: Values OK - no overflow");
    }
    else {
      TelnetStream.println("INA226: Overflow! Choose higher current range");
    }
    TelnetStream.println();
  }
}

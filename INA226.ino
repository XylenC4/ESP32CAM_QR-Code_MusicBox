#include <Wire.h>
#include <INA226.h>

INA226 ina;

float INA226_BusVoltage, INA226_BusPower, INA226_ShuntVoltage, INA226_ShuntCurrent;

// constants won't change:
const long interval = 5000;           // interval at which to blink (milliseconds)

void checkConfig()
{
  TelnetStream.print("Mode:                  ");
  switch (ina.getMode())
  {
    case INA226_MODE_POWER_DOWN:      TelnetStream.println("Power-Down"); break;
    case INA226_MODE_SHUNT_TRIG:      TelnetStream.println("Shunt Voltage, Triggered"); break;
    case INA226_MODE_BUS_TRIG:        TelnetStream.println("Bus Voltage, Triggered"); break;
    case INA226_MODE_SHUNT_BUS_TRIG:  TelnetStream.println("Shunt and Bus, Triggered"); break;
    case INA226_MODE_ADC_OFF:         TelnetStream.println("ADC Off"); break;
    case INA226_MODE_SHUNT_CONT:      TelnetStream.println("Shunt Voltage, Continuous"); break;
    case INA226_MODE_BUS_CONT:        TelnetStream.println("Bus Voltage, Continuous"); break;
    case INA226_MODE_SHUNT_BUS_CONT:  TelnetStream.println("Shunt and Bus, Continuous"); break;
    default: TelnetStream.println("unknown");
  }

  TelnetStream.print("Samples average:       ");
  switch (ina.getAverages())
  {
    case INA226_AVERAGES_1:           TelnetStream.println("1 sample"); break;
    case INA226_AVERAGES_4:           TelnetStream.println("4 samples"); break;
    case INA226_AVERAGES_16:          TelnetStream.println("16 samples"); break;
    case INA226_AVERAGES_64:          TelnetStream.println("64 samples"); break;
    case INA226_AVERAGES_128:         TelnetStream.println("128 samples"); break;
    case INA226_AVERAGES_256:         TelnetStream.println("256 samples"); break;
    case INA226_AVERAGES_512:         TelnetStream.println("512 samples"); break;
    case INA226_AVERAGES_1024:        TelnetStream.println("1024 samples"); break;
    default: TelnetStream.println("unknown");
  }

  TelnetStream.print("Bus conversion time:   ");
  switch (ina.getBusConversionTime())
  {
    case INA226_BUS_CONV_TIME_140US:  TelnetStream.println("140uS"); break;
    case INA226_BUS_CONV_TIME_204US:  TelnetStream.println("204uS"); break;
    case INA226_BUS_CONV_TIME_332US:  TelnetStream.println("332uS"); break;
    case INA226_BUS_CONV_TIME_588US:  TelnetStream.println("558uS"); break;
    case INA226_BUS_CONV_TIME_1100US: TelnetStream.println("1.100ms"); break;
    case INA226_BUS_CONV_TIME_2116US: TelnetStream.println("2.116ms"); break;
    case INA226_BUS_CONV_TIME_4156US: TelnetStream.println("4.156ms"); break;
    case INA226_BUS_CONV_TIME_8244US: TelnetStream.println("8.244ms"); break;
    default: TelnetStream.println("unknown");
  }

  TelnetStream.print("Shunt conversion time: ");
  switch (ina.getShuntConversionTime())
  {
    case INA226_SHUNT_CONV_TIME_140US:  TelnetStream.println("140uS"); break;
    case INA226_SHUNT_CONV_TIME_204US:  TelnetStream.println("204uS"); break;
    case INA226_SHUNT_CONV_TIME_332US:  TelnetStream.println("332uS"); break;
    case INA226_SHUNT_CONV_TIME_588US:  TelnetStream.println("558uS"); break;
    case INA226_SHUNT_CONV_TIME_1100US: TelnetStream.println("1.100ms"); break;
    case INA226_SHUNT_CONV_TIME_2116US: TelnetStream.println("2.116ms"); break;
    case INA226_SHUNT_CONV_TIME_4156US: TelnetStream.println("4.156ms"); break;
    case INA226_SHUNT_CONV_TIME_8244US: TelnetStream.println("8.244ms"); break;
    default: TelnetStream.println("unknown");
  }

  TelnetStream.print("Max possible current:  ");
  TelnetStream.print(ina.getMaxPossibleCurrent());
  TelnetStream.println(" A");

  TelnetStream.print("Max current:           ");
  TelnetStream.print(ina.getMaxCurrent());
  TelnetStream.println(" A");

  TelnetStream.print("Max shunt voltage:     ");
  TelnetStream.print(ina.getMaxShuntVoltage());
  TelnetStream.println(" V");

  TelnetStream.print("Max power:             ");
  TelnetStream.print(ina.getMaxPower());
  TelnetStream.println(" W");
}

void setup_INA226()
{
  //Wire.begin(14, 2);
  TelnetStream.println("Initialize INA226");
  TelnetStream.println("-----------------------------------------------");

  // Default INA226 address is 0x40
  ina.begin();

  // Configure INA226
  ina.configure(INA226_AVERAGES_1, INA226_BUS_CONV_TIME_1100US, INA226_SHUNT_CONV_TIME_1100US, INA226_MODE_SHUNT_BUS_CONT);

  // Calibrate INA226. Rshunt = 0.01 ohm, Max excepted current = 4A
  ina.calibrate(0.1, 4);

  // Display configuration
  checkConfig();

  //  TelnetStream.println("-----------------------------------------------");
  //
  //  xTaskCreatePinnedToCore(
  //    taskINA226,          /* Task function. */
  //    "taskINA226",        /* String with name of task. */
  //    5000,            /* Stack size in bytes. */
  //    NULL,             /* Parameter passed as input of the task */
  //    1,                /* Priority of the task. */
  //    NULL,
  //    1);            /* Task handle. */
}

void taskINA226( void * parameter )
{
  for (;;)
  {
    INA226_BusVoltage = ina.readBusVoltage();
    INA226_BusPower = ina.readBusPower();
    INA226_ShuntVoltage = ina.readShuntVoltage();
    INA226_ShuntCurrent = ina.readShuntCurrent();

    //    TelnetStream.print("Bus voltage:   ");
    //    TelnetStream.print(ina.readBusVoltage(), 5);
    //    TelnetStream.println(" V");

    //    TelnetStream.print("Bus power:     ");
    //    TelnetStream.print(ina.readBusPower(), 5);
    //    TelnetStream.println(" W");
    //
    //    TelnetStream.print("Shunt voltage: ");
    //    TelnetStream.print(ina.readShuntVoltage(), 5);
    //    TelnetStream.println(" V");
    //
    //    TelnetStream.print("Shunt current: ");
    //    TelnetStream.print(ina.readShuntCurrent(), 5);
    //    TelnetStream.println(" A");

    TelnetStream.println("");
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
  vTaskDelete( NULL );
}

void loop_INA226() {
  static unsigned long previousMillis = 0;        // will store last time LED was updated
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    // save the last time you blinked the LED
    previousMillis = currentMillis;

    INA226_BusVoltage = ina.readBusVoltage();
    INA226_BusPower = ina.readBusPower();
    INA226_ShuntVoltage = ina.readShuntVoltage();
    INA226_ShuntCurrent = ina.readShuntCurrent();

    TelnetStream.print("Bus voltage:   ");
    TelnetStream.print(ina.readBusVoltage(), 5);
    TelnetStream.println(" V");

    TelnetStream.print("Bus power:     ");
    TelnetStream.print(ina.readBusPower(), 5);
    TelnetStream.println(" W");

    TelnetStream.print("Shunt voltage: ");
    TelnetStream.print(ina.readShuntVoltage(), 5);
    TelnetStream.println(" V");

    TelnetStream.print("Shunt current: ");
    TelnetStream.print(ina.readShuntCurrent(), 5);
    TelnetStream.println(" A");
  }
}

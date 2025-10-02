#include <Arduino.h>

// NOTE перенесено из проекта Arduino Nano V3, нужно адаптировать пины для ESP8266

// PINOUT
const int pin_tempersature = 1; // A1
const int pin_flow_sensor = 2;  // D2
const int pin_TDS = 3;          // A2

#include "NTCTemperatureProbe.h"
const float base_temp_resistor = 100*KOm; // 100KOm
const float VCC = 3.3; // 

MH_02_TemperatureProbe temp_sensor(pin_tempersature, base_temp_resistor);

#include <LCDI2C_Multilingual.h>
LCDI2C_Russian lcd(0x27, 16, 2);    // I2C address = 0x27; LCD = Surenoo SLC1602A (European)

// variables
float target_temperature = 32.0;

// Датчик расхода воды YF-S201, pulses_per_liter = 450
#include "FlowSensor.h"
FlowSensor flowDF = MH_01_Flow(pin_flow_sensor);  // D2, 450 pulses_per_liter

// TDS
#include "TDS.h"
float tds_value = -1;
TDSMeter tds(pin_TDS, VCC);

// DECLARATION
void display_1602(float t1, float tTarget);

// INTERRUPTS
//  Функция-обработчик прерывания от датчика потока
void flowSensorDeflegmatorInterrupt() {
    flowDF.pulse(); //  Увеличиваем счетчик импульсов
}

// SETUP
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  //ds.setResolution(12);
  //ds.requestTemp();  // первый запрос на измерение
  pinMode(pin_tempersature, INPUT);

  lcd.init();                             // Initialize the LCD
  lcd.backlight();                        // Turn on the LCD backlight
  lcd.print("Init");

  // flow sensor
  flowDF.init(flowSensorDeflegmatorInterrupt);
}

void loop() 
{
  float t = temp_sensor.get_temperature();  
  flowDF.tick();
  if(tds.tick(t)) {
    tds_value = tds.getTdsValue();
  }
  
  display_1602(t, 0);
}


void trace(float tC)
{
  Serial.print(tC, 1);
  Serial.print("ºC");
  Serial.print(" | ");
  Serial.print(target_temperature, 1);
  Serial.print("ºC");
  Serial.println();
}

void display_1602(float t1, float tTarget)
{
  lcd.setCursor(0, 0);
  lcd.print(t1, 1); lcd.write(0xDF); // Print the degree symbol
 // lcd.print("C");
  if( t1 < tTarget){
    lcd.print(" > ");
  } else if(t1 > tTarget){
    lcd.print(" < ");
  }
  else{
    lcd.print(" | ");
  }
 // lcd.print(tTarget, 1); lcd.write(0xDF); // Print the degree symbol
  lcd.print("TDS ");
  if(tds_value >= 0)
  {
    lcd.print(tds_value, 0); lcd.println("  ");
  }
  else
  {
    lcd.println("---");
  }

 // Расход воды
  lcd.setCursor(0,1);
  lcd.print(flowDF.get_pulses_total());
  lcd.print("->");
//  lcd.println(FlowSensor::format_rate(flowDF.flow_rate_minute()));
  lcd.print(flowDF.volume_liters(), 3);    // total water volume
  lcd.println("L");
}


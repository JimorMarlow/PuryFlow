#include "FlowSensor.h"

FlowSensor::FlowSensor(int pin, float ppl_value)
: pinPulse_(pin)
, pulses_per_liter_(ppl_value)
{

}

bool FlowSensor::init(void (*pHandler)())
{
  // flow sensor
   pinMode(pinPulse_, INPUT_PULLUP); //  Настраиваем пин как вход с подтягивающим резистором
  //  Устанавливаем обработчик прерывания на rising edge (срабатывает при переходе от низкого уровня к высокому)
  attachInterrupt(digitalPinToInterrupt(pinPulse_), pHandler, RISING);

  return true;
}

void FlowSensor::reset()
{
  pulses_total_ = 0;
  samples_ = stats_t();
  flow_rate_minute_ = 0.0;
}

void FlowSensor::tick()
{
  stats_t data = samples_;  // Взять все данные для расчетов
  if(!data.prev_sample.empty() && !data.last_sample.empty())
  {
    // check for timeout
    if(data.prev_sample.timestamp_ms == data.last_sample.timestamp_ms)
    {
      if((millis() - data.last_sample.timestamp_ms) > timeout_ms_)
      {
        // timeout - reset stat
        samples_ = stats_t();
        flow_rate_minute_ = 0.0;
      }
    }
    else
    {
      // Calc flow rate
      float volume_interval = (data.last_sample.pulse - data.prev_sample.pulse) / pulses_per_liter_; // объем воды в литрах
      float time_interval_ms = data.last_sample.timestamp_ms - data.prev_sample.timestamp_ms; // вреия интервала в мс
      float rate = volume_interval / time_interval_ms; // скорость потока на интервале
      flow_rate_minute_ = rate * (60000.0 / time_interval_ms); // скорость потока в минуту
      samples_.prev_sample = data.last_sample; // этот отсчет обработали
    //  Serial.print("volume_interval = "); Serial.println(volume_interval, 6); 
    //  Serial.print("time_interval_ms = "); Serial.println(time_interval_ms); 
    //  Serial.print("rate = "); Serial.println(rate,6); 
    //  Serial.print("flow_rate_minute_ = "); Serial.println(flow_rate_minute_,6); 
    }
  }
  else
  {
    // Reset flow rate
    flow_rate_minute_ = 0.0;
  }
}

float FlowSensor::volume_liters()   // total water volume
{
  return pulses_total_ / pulses_per_liter_;
}

int FlowSensor::get_pulses_total() const   // total pulse counter
{
  return pulses_total_;
}

float FlowSensor::get_pulses_per_liter() const
{
  return pulses_per_liter_;
}

void FlowSensor::pulse() // called from interrupt handler
{
  int pulses_count = ++pulses_total_;
  samples_.last_sample = sample_t(pulses_count, millis());
  if(samples_.prev_sample.empty())
  {
    samples_.prev_sample = samples_.last_sample;
  }
}

float FlowSensor::flow_rate_minute()
{
  return flow_rate_minute_;
}

String FlowSensor::format_rate(float rate)
{
  String units = "L/m  ";
  int decimal = 0;
  float ratio = 1.0;

  if(rate < 10.0){
    decimal = 1;
  }
  
  if(rate < 1.0){
    decimal= 2;
  }
  
  if(rate < 0.1){
    ratio = 1000.0; decimal = 1; units = "mL/m";
  }
  
  if(rate < 0.001){
    ratio = 1000000.0; decimal = 0; units = "uL/m";
  }

  if(rate < 0.000001){
    ratio = 1000000000.0; decimal = 0; units = "nL/m";
  }

  if(rate == 0)
  {
    units = " L/m";
  }

  String v((rate * ratio), decimal);
//  Serial.print("format_rate("); Serial.print(rate,6); Serial.print(") "); Serial.print(v); Serial.println(units); 
//  Serial.print("ratio = "); Serial.print(ratio); Serial.print(", decimal = "); Serial.println(decimal);
  return v+units;
}


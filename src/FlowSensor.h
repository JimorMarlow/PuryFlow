// FlowSensor.h
// Base class for water flow sensors
// YF-S201, pulses_per_liter = 450
// Jimor Marlow (c) 2025 jimor@inbox.ru

#ifndef FlowSensor_h
#define FlowSensor_h

#include <Arduino.h>

/////////////////////////////////////////////////////////////////
class FlowSensor {
  public:
    FlowSensor(int pin, float ppl_value);
    virtual ~FlowSensor() = default;

    // Operations
    virtual bool init(void (*pHandler)());
    void tick();  // calc current flow rate
    void reset();   // reset all stats and start collecting new data
    float volume_liters();    // total water volume
    float flow_rate_minute();      

    virtual void pulse(); // called from interrupt handler

    // Attributes
    int get_pulses_total() const;   // total pulse counter
    float get_pulses_per_liter() const;
    static String format_rate(float rate);

    struct sample_t
    {
      int pulse = -1;  // hall sensor value
      unsigned int timestamp_ms = 0;  // timestamp in ms

      sample_t() = default;
      sample_t(int tick, unsigned int time_ms) : pulse(tick), timestamp_ms(time_ms) {}
      void reset() { pulse = -1; timestamp_ms = 0;}
      bool empty() const { return pulse == -1; }
    };

    struct stats_t
    {
      sample_t last_sample, prev_sample;
    };

  protected:
    // setup
    int pinPulse_; // Hall sensor input  
    float pulses_per_liter_ = 1.0; // Sensor's sens parameter
    unsigned int timeout_ms_ = 10 * 1000;  // reset time - inactivity timter

    // statistics
    volatile int pulses_total_ = 0; // total pulse counter
    stats_t samples_;
    volatile float flow_rate_minute_ = 0.0;
  
  
};

class YF_S201 : public FlowSensor
{
public:
  YF_S201(int pin) 
  : FlowSensor(pin, 450.0) {} 
};

class MH_01_Flow : public FlowSensor
{
public:
  MH_01_Flow(int pin) 
  : FlowSensor(pin, 1720.0) {} // 2.0 - 5400, 3.5 - 2580
};
///////////////////////////////////////////////////////////////////

#endif

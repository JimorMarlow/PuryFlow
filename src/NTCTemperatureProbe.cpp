#include "NTCTemperatureProbe.h"
#include "Arduino.h"

NTCTemperatureProbe::NTCTemperatureProbe(int pin,  // Разъем подключенного датчика
        float probeResistance /*= 100000*/, // Внутренне сопростивление термистора (100КОм)
        float baseResitance /*= 100000*/)
        : pin_(pin)
        , probeResistance_(probeResistance)
        , baseResitance_(baseResitance)
{
    pinMode(pin_, INPUT);
    avg_temperature_.setStep(500);
}

NTCTemperatureProbe::~NTCTemperatureProbe()
{

}

float NTCTemperatureProbe::get_temperature()
{
    int t_raw = analogRead(pin_);
    Serial.println(t_raw);
    // вычисляем сопротивление датчика термистора
    float resistance = baseResitance_ / (1023.0 / t_raw - 1);
   // float tt = (1023.0 / t_raw - 1);
   // tt = baseResitance_;
    Serial.print("resistance = "); Serial.println(resistance);

    // Расчет температуры по уравнению Стейнхарта-Харта
  //  float temperature = 1.0 / (1.0 / (referenceTemp_+273.15) + (1.0/beta_factor_)*log(resistance / referenceTemp_));
  //  temperature -= 273.15; // Кельвины в Цельсии

    float t = temperature_.filtered(resistance_to_temperature(resistance));
//    t = avg_temperature_.filteredTime(t);

    return t;
}

float mapf(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

float NTCTemperatureProbe::resistance_to_temperature(float res_value)
{
    ref_signal_t* table = nullptr;
    int count = get_lookup_table(&table);
    if(count > 0 && table != nullptr)
    {
        if(count == 1) return table[0].t; // ниже первого
        for(int i = 0; i < count - 1; ++i)
        {
            if(table[i].r <= res_value && res_value > table[i+1].r)
            {
                float value = mapf(res_value, 
                    table[i].r, table[i+1].r, 
                    table[i].t, table[i+1].t
                );
                return value;
            }
        }
        // Не нашли хначение - возвращаем верхенн
        return table[count-1].t;
    }
    return -50.0f;
}

//////////////////////////////////////////////////////
ref_signal_t REF3950_50KOm[] = {
    // tC, resistance KOm
    {-20, 483846.3},
    { 0, 163008.6},
    { 5, 126845.8},
    {10, 99458.9},
    {15, 78550.5},
    {19, 65359.5},
    {20, 62465.3},
    {21, 59714.8},
    {22, 57100.2},
    {23, 54614.1},
    {24, 52249.6},
    {25, 50000.0},
    {26, 47859.2},
    {27, 45821.4},
    {28, 43881.1},
    {29, 42033.1},
    {30, 40272.6},
    {31, 38595.0},
    {32, 36996.1},
    {33, 35471.6},
    {34, 34017.9},
    {35, 32631.2},
    {40, 26590.3}

};

int MH_02_TemperatureProbe::get_lookup_table(ref_signal_t** pHead)
 { // Таблица перевода значений
    *pHead = REF3950_50KOm;
    return sizeof(REF3950_50KOm)/sizeof(REF3950_50KOm[0]);
}
    
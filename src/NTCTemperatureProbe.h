// Усоежг
#include "GyverFilters.h"
#include "InitFilter.h"

// Чтение температуры с NTC датчика
const float KOm = 1000.0f;

struct ref_signal_t
{
    float t;  // temperature
    float r;  // resistance
};

class NTCTemperatureProbe
{
public:
    NTCTemperatureProbe(int pin,  // Разъем подключенного датчика
        float probeResistance /*= 100000*/, // Внутренне сопростивление термистора (100КОм)
        float baseResitance /*= 100000*/);
    virtual ~NTCTemperatureProbe();

    float get_temperature(); // получить значенин фильтрованной температуры в цельсиях

protected:
    virtual int get_lookup_table(ref_signal_t** pHead) = 0; // Таблица перевода значений
    virtual float resistance_to_temperature(float res_value);

protected:
    int pin_ = -1;  // Разъем подключенного датчика
    float probeResistance_ = 100*KOm; // Внутренне сопростивление термистора (100КОм)
    float baseResitance_ = 100*KOm; // Сопростивление балансировочного резистора
    float beta_factor_ = 3950.0;  // beta-коэффициент
    float referenceTemp_ = 25.0; // Температура для опорного сопротиалннч

    InitFilter<GMedian3<float>, float, 3> temperature_; 		
    GFilterRA avg_temperature_;
};

// Температурный датчик, интегрированный в DSW MH-02, NTC 3950 50KOm
class MH_02_TemperatureProbe : public NTCTemperatureProbe
{
public:
    MH_02_TemperatureProbe(int pin,  // Разъем подключенного датчика
        float baseResitance)
        : NTCTemperatureProbe(pin, 50*KOm, baseResitance)
    {}

    virtual ~MH_02_TemperatureProbe() = default;
protected:
    virtual int get_lookup_table(ref_signal_t** pHead) override; // Таблица перевода значений
};
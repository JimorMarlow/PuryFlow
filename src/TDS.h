// Total Dissolved Solid - измерение количества расстворенных твердых солей в воде
// источник https://wiki.dfrobot.com/Gravity__Analog_TDS_Sensor___Meter_For_Arduino_SKU__SEN0244
// redpanther/CQRobotTDS

const int SCOUNT = 30;           // sum of sample point

class TDSMeter
{
public:
    TDSMeter(int input_pin, float vref = 5.0);
    virtual ~TDSMeter() = default;

    bool tick(); //read and calculate
	bool tick(float temperature); //read and calculate
	void setTemperature(float temperature);  //set the temperature and execute temperature compensation
	void setAdcRange(float range);  //1024 for 10bit ADC;4096 for 12bit ADC
	float getTdsValue();
    bool isTdsValue() const;

protected:
    int pin_ = -1;
	float aref_ = 5.0;  // default 5.0V on Arduino UNO
	float adcRange_ = 1024.0;
	float temperature_ = 25.0;
 
	float tds_value_;
    bool  is_tds_value_ = false;
    
    int analogBuffer[SCOUNT];    // store the analog value in the array, read from ADC
    int analogBufferTemp[SCOUNT];
    int analogBufferIndex = 0, copyIndex = 0;
    float averageVoltage = 0, tdsValue = 0, mtemperature = 25;

protected:
    int getMedianNum(int bArray[], int iFilterLen) ;
};
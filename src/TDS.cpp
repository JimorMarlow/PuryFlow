// 
#include "TDS.h"
#include "Arduino.h"

TDSMeter::TDSMeter(int input_pin, float vref /*= 5.0*/)
: pin_(input_pin)
, aref_ (vref)
{
    pinMode(pin_, INPUT);
}

bool TDSMeter::isTdsValue() const
{
    return is_tds_value_;
}

bool TDSMeter::tick(float temperature) //read and calculate
{
    setTemperature(temperature);
    return tick();
}

bool TDSMeter::tick()    // true - есть вычисленное значение для текущей температуры воды
{
    static unsigned long analogSampleTimepoint = millis();
	if (millis() - analogSampleTimepoint > 40U)  //every 40 milliseconds,read the analog value from the ADC
	{
		analogSampleTimepoint = millis();
		this->analogBuffer[this->analogBufferIndex] = analogRead(pin_);    //read the analog value and store into the buffer
		this->analogBufferIndex++;
		if (this->analogBufferIndex == SCOUNT)
			this->analogBufferIndex = 0;
	}
	static unsigned long printTimepoint = millis();
	if (millis() - printTimepoint > 800U)
	{
		printTimepoint = millis();
		for (copyIndex = 0; copyIndex < SCOUNT; copyIndex++)
			this->analogBufferTemp[copyIndex] = this->analogBuffer[copyIndex];
		averageVoltage = getMedianNum(analogBufferTemp, SCOUNT) * aref_ / adcRange_; // read the analog value more stable by the median filtering algorithm, and convert to voltage value
		float compensationCoefficient = 1.0 + 0.02 * (temperature_ - 25.0); //temperature compensation formula: fFinalResult(25^C) = fFinalResult(current)/(1.0+0.02*(fTP-25.0));
		float compensationVolatge = averageVoltage / compensationCoefficient; //temperature compensation
		tds_value_ = (133.42 * compensationVolatge * compensationVolatge * compensationVolatge - 255.86 * compensationVolatge * compensationVolatge + 857.39 * compensationVolatge) * 0.5; //convert voltage value to tds value
        is_tds_value_ = true;
	}
	
    return is_tds_value_;
}

int TDSMeter::getMedianNum(int bArray[], int iFilterLen) 
{
    int bTab[iFilterLen];
    for (byte i = 0; i<iFilterLen; i++)
        bTab[i] = bArray[i];
    int i, j, bTemp;
    for (j = 0; j < iFilterLen - 1; j++) 
    {
        for (i = 0; i < iFilterLen - j - 1; i++) 
        {
        if (bTab[i] > bTab[i + 1]) 
            {
                bTemp = bTab[i];
                bTab[i] = bTab[i + 1];
                bTab[i + 1] = bTemp;
            }
        }
    }
    if ((iFilterLen & 1) > 0)
        bTemp = bTab[(iFilterLen - 1) / 2];
    else
        bTemp = (bTab[iFilterLen / 2] + bTab[iFilterLen / 2 - 1]) / 2;
    return bTemp;
}

void TDSMeter::setTemperature(float temperature)  //set the temperature and execute temperature compensation
{
	temperature_ = temperature;
}

void TDSMeter::setAdcRange(float range)  //1024 for 10bit ADC;4096 for 12bit ADC
{
	adcRange_ = range;
}


float TDSMeter::getTdsValue()
{
    is_tds_value_ = false;
	return tds_value_;
}
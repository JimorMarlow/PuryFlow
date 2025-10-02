#### PuryFlow

Note: Тестовый поект для изучения платформы Arduino.
(c) Jimor Marlow

## Измерение потока воды с устройства очистки ОСМОС для контроля качества получаемой воды и сроков замены фильтрующих картриджей (фильтров)
Базируется на устройстве **MH-01**, интегрированный счетчик воды, датчик температуры (для коррекции значений TDS), датчик TDS (количество растворенных твердых солей в воде)

<small>AliExpress</small>: https://sl.aliexpress.ru/p?key=nowYVtR
*"Multi-Function Flow Sensor MH-01 MH-02 TDS Temperature Hall Effect 1/4" & 3/8" Quick Connect High Accuracy - MH-01-G1-4"*

MH-01 (3-in-1) models come with flow, TDS, and temperature sensing functions, using an integrated 6-wire cable only.
All models operate on DC 5 18V and offer ±3% flow detection accuracy.

| Parameters  | MH-01 (3-in-1, 6-wire) |
| ------------- |:-------------:|
| Function|Flow + TDS + Temperature| 
| Cable Type|6-wire Integrated Only| 
| Bore Size|4mm| 
| Pipe Interface|6.35mm PE pipe| 
| Operating Voltage|DC 5~18V| 
| Minimum Rated Voltage|DC 3.5V| 
| Max Working Current|15mA (DC 5V)| 
| Load Capacity|10mA| 
| Temperature Detection|Yes| 
| Operating Temperature Range|80°C| 
| Operating Humidity|35% 90% RH (no frost)| 
| Storage Temperature|-25~+80°C| 
| Storage Humidity|25% 95% RH| 
| Max Pressure|0.8MPa| 
| Flow Detection Accuracy|0.3~6L/min ±3%| 
| Flow Range Details|0.3 6L/min| 
| Pulse Output Duty Cycle|50 ±10%| 
| Output Rise Time|0.04μs| 
| Output Fall Time|0.18μs| 
| Pulse Frequency Formula|Hz = 36 × Q + 3% (Q=L/min)| 

##### Измерение температуры выполняется для NTC датчика
все таблиы в Docs\TemperatureSensor_3950_50K

##### TDS (Total Dissolved Solids) - Docs\TDS
для измерения была приобретена плата "TDS Meter 1.0" https://sl.aliexpress.ru/p?key=wwZYVc5
2-pin input подается с платы MH-01, а 3-pin output в ардуино


2025, Jimor Marlow

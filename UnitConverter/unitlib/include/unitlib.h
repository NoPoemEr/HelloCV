#ifndef UNITLIB_H
#define UNITLIB_H
// 单位转换库：温度、长度、质量三组单位之间的换算。
namespace unitlib {
// ---------- 温度：摄氏度 <-> 华氏度 ----------
double celsiusToFahrenheit(double celsius);
double fahrenheitToCelsius(double fahrenheit);
// ---------- 长度：米 <-> 英尺 ----------
double meterToFoot(double meter);
double footToMeter(double foot);
// ---------- 质量：千克 <-> 磅 ----------
double kilogramToPound(double kilogram);
double poundToKilogram(double pound);
}
#endif

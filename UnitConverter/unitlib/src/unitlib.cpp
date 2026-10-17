#include "unitlib.h"
//   1 英尺 = 0.3048 米（国际英尺定义）
//   1 磅   = 0.45359237 千克（国际磅定义）
//   温度：F = C * 9 / 5 + 32
namespace unitlib {
double celsiusToFahrenheit(double celsius) {
    return celsius * 9.0 / 5.0 + 32.0;
}

double fahrenheitToCelsius(double fahrenheit) {
    return (fahrenheit - 32.0) * 5.0 / 9.0;
}

double meterToFoot(double meter) {
    return meter / 0.3048;
}

double footToMeter(double foot) {
    return foot * 0.3048;
}

double kilogramToPound(double kilogram) {
    return kilogram / 0.45359237;
}

double poundToKilogram(double pound) {
    return pound * 0.45359237;
}

}
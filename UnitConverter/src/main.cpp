#include <iomanip>
#include <iostream>
#include <limits>

#include "unitlib.h"

namespace {

// 丢掉这一行剩下的字符，同时清掉 cin 的失败状态
void discardLine() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

// 1：读取成功；0：输入非法；-1：输入结束（Ctrl+D）
int readInt(int& value) {
    if (std::cin >> value) {
        return 1;
    }
    if (std::cin.eof()) {
        return -1;
    }
    discardLine();
    std::cout << "请输入菜单中的编号。\n";
    return 0;
}

int readDouble(double& value) {
    if (std::cin >> value) {
        return 1;
    }
    if (std::cin.eof()) {
        return -1;
    }
    discardLine();
    std::cout << "请输入一个数字。\n";
    return 0;
}

void printValue(double value) {
    std::cout << std::fixed << std::setprecision(4) << value;
}

struct Conversion {
    const char* from;
    const char* to;
    double (*apply)(double);
};

// 一类单位共用一个子菜单,选方向、读数、输出结果
void runCategory(const char* title, const Conversion* items, int count) {
    while (true) {
        std::cout << "\n--- " << title << " ---\n";
        for (int i = 0; i < count; ++i) {
            std::cout << i + 1 << ". " << items[i].from << " -> "
                      << items[i].to << '\n';
        }
        std::cout << "0. 返回主菜单\n请选择：";

        int choice = 0;
        int status = readInt(choice);
        if (status == -1) {
            return;
        }
        if (status == 0) {
            continue;
        }
        if (choice == 0) {
            return;
        }
        if (choice < 1 || choice > count) {
            std::cout << "没有这个选项。\n";
            continue;
        }

        std::cout << "请输入要转换的数值：";
        double value = 0.0;
        status = readDouble(value);
        if (status == -1) {
            return;
        }
        if (status == 0) {
            continue;
        }

        const Conversion& item = items[choice - 1];
        std::cout << '\n';
        printValue(value);
        std::cout << ' ' << item.from << " = ";
        printValue(item.apply(value));
        std::cout << ' ' << item.to << "\n\n";
    }
}

}  // namespace

int main() {
    const Conversion temperature[] = {
        {"摄氏度", "华氏度", unitlib::celsiusToFahrenheit},
        {"华氏度", "摄氏度", unitlib::fahrenheitToCelsius},
    };
    const Conversion length[] = {
        {"米", "英尺", unitlib::meterToFoot},
        {"英尺", "米", unitlib::footToMeter},
    };
    const Conversion mass[] = {
        {"千克", "磅", unitlib::kilogramToPound},
        {"磅", "千克", unitlib::poundToKilogram},
    };

    while (true) {
        std::cout << "\n===== 单位转换器 =====\n"
                  << "1. 温度（摄氏度 / 华氏度）\n"
                  << "2. 长度（米 / 英尺）\n"
                  << "3. 质量（千克 / 磅）\n"
                  << "0. 退出\n"
                  << "请选择：";

        int choice = 0;
        int status = readInt(choice);
        if (status == -1) {
            std::cout << "\n已退出。\n";
            return 0;
        }
        if (status == 0) {
            continue;
        }

        switch (choice) {
            case 0:
                std::cout << "已退出。\n";
                return 0;
            case 1:
                runCategory("温度", temperature, 2);
                break;
            case 2:
                runCategory("长度", length, 2);
                break;
            case 3:
                runCategory("质量", mass, 2);
                break;
            default:
                std::cout << "没有这个选项。\n";
                break;
        }
    }
}

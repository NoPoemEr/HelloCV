# HelloCV

个人编程训练仓库：把每个练习拆成独立目录，各自可单独构建、运行、测试。

| 目录 | 内容 | 技术栈 |
| --- | --- | --- |
| [`UnitConverter/`](UnitConverter/) | CMake 实践：命令行单位转换器（可执行程序 + 静态库） | C++11 / CMake |
| [`llm-gateway/`](llm-gateway/) | AI 实践：石头剪刀布决策网关（一行 JSON 进、一行 JSON 出） | Python 3 |

## 项目一：CMake 单位转换器

用 C++ 写一个命令行单位转换器，练习把项目拆成"可执行程序 + 静态库"，并用 CMake 组织多目录构建。

支持三组双向转换：

- 温度：摄氏度 ↔ 华氏度
- 长度：米 ↔ 英尺
- 质量：千克 ↔ 磅

### 项目结构

```
HelloCV/
├── README.md
├── notes.md
├── .gitignore
└── UnitConverter/
    ├── CMakeLists.txt
    ├── src/
    │   └── main.cpp
    ├── unitlib/
    │   ├── CMakeLists.txt
    │   ├── include/
    │   │   └── unitlib.h
    │   └── src/
    │       └── unitlib.cpp
    └── build/              # 本地构建目录，已被 .gitignore 忽略
```

各文件的作用：

- `UnitConverter/CMakeLists.txt`：项目入口。设置 C++ 标准、默认构建类型，组装库和可执行文件
- `UnitConverter/src/main.cpp`：主菜单、子菜单、输入输出，调用库里的转换函数
- `UnitConverter/unitlib/CMakeLists.txt`：把 `unitlib.cpp` 编译成静态库
- `UnitConverter/unitlib/include/unitlib.h`：6 个转换函数的声明
- `UnitConverter/unitlib/src/unitlib.cpp`：转换函数的具体实现
- `UnitConverter/build/`：CMake 生成的构建目录，不提交

### 环境要求

- CMake 3.10 及以上（本项目在 3.22.1 上测试）
- 支持 C++11 的编译器（测试用 g++ 11.4.0）
- make（或 ninja）
- Linux，测试环境为 Ubuntu 22.04

### 环境配置

Debian / Ubuntu 下一条命令装齐编译器和构建工具：

```bash
sudo apt update
sudo apt install -y build-essential cmake
```

装完确认版本：

```bash
cmake --version    # 需要 3.10 以上
g++ --version      # 需要支持 C++11
make --version
```

如果系统自带的 CMake 低于 3.10，可以用官方安装脚本装新版：

```bash
wget -O cmake.sh https://github.com/Kitware/CMake/releases/download/v3.22.1/cmake-3.22.1-linux-x86_64.sh
sudo bash cmake.sh --prefix=/usr/local --skip-license
```

### 构建

进入 `UnitConverter/` 目录后执行：

```bash
cd UnitConverter

# 配置并生成构建文件
cmake -S . -B build

# 编译
cmake --build build -j
```

构建产物：

```
build/unitconverter          可执行文件
build/unitlib/libunitlib.a   静态库
```

### 运行

```bash
./build/unitconverter
```

主菜单输入 `1`、`2`、`3` 选择单位类别，输入 `0` 退出；子菜单输入 `1`、`2` 选择转换方向，输入 `0` 返回主菜单。输入非数字时会提示并重新询问，不会退出程序。

示例：

```
===== 单位转换器 =====
1. 温度（摄氏度 / 华氏度）
2. 长度（米 / 英尺）
3. 质量（千克 / 磅）
0. 退出
请选择：1

--- 温度 ---
1. 摄氏度 -> 华氏度
2. 华氏度 -> 摄氏度
0. 返回主菜单
请选择：1
请输入要转换的数值：100

100.0000 摄氏度 = 212.0000 华氏度
```

结果统一保留 4 位小数。

### 换算公式

| 转换 | 公式 |
|---|---|
| 摄氏度 → 华氏度 | F = C × 9 / 5 + 32 |
| 华氏度 → 摄氏度 | C = (F − 32) × 5 / 9 |
| 米 → 英尺 | ft = m ÷ 0.3048 |
| 英尺 → 米 | m = ft × 0.3048 |
| 千克 → 磅 | lb = kg ÷ 0.45359237 |
| 磅 → 千克 | kg = lb × 0.45359237 |

### 清理

```bash
rm -rf build
```

只想删编译产物、保留配置的话：

```bash
cmake --build build --target clean
```

## 项目二：LLM 决策网关

把一行 JSON 请求转发给 DeepSeek，再把模型返回的任意文本整理成一行严格合法的 JSON，
用于石头剪刀布决策。核心是三级 JSON 容错、重试分类和本地兜底：坏输入、上游报错、
模型胡说都不能让程序崩掉。

```bash
cd llm-gateway

python3 -m venv .venv && source .venv/bin/activate
pip install -r requirements.txt
export DEEPSEEK_API_KEY=sk-xxxxxxxx        # key 只放环境变量

python selfcheck.py                        # 93 个测试
echo '{"time": "t1", "history": ["rock"]}' | python run_gateway.py --offline
python play_game.py --rounds 10            # 真实 API 对局
```

目录结构、协议、设计要点和完整用法见 [`llm-gateway/README.md`](llm-gateway/README.md)。

## 语雀笔记

每次任务的学习笔记与实践笔记，链接汇总见 [notes.md](notes.md)。

## 仓库

https://github.com/NoPoemEr/HelloCV

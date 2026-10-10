# HelloCV
Git 实践任务交付仓库。本仓库汇总本次任务的两个成果仓库：


| 仓库 | 平台 | 地址 |
|---|---|---|
| git_training | Gitee | https://gitee.com/Argentum_071/git_training |
HelloCV | GitHub | https://github.com/NoPoemEr/HelloCV |

`git_training` 是用 C++ 实现的控制台四则运算计算器，也是本次练习 Git
全部基本命令的主仓库；`HelloCV`（即本仓库）用于交付说明和语雀笔记汇总。

## 一、任务说明

本次实践任务包含两部分：

1. 配置 Git 环境，包括全局用户名邮箱和 Gitee 的 SSH 密钥；
2. 在 Gitee 的 `git_training` 仓库中完成一个支持加、减、乘、除的
   计算器，并通过分支开发的方式练习 Git 命令；
3. 将过程记录写成语雀笔记，并在本仓库中汇总所有笔记链接。

## 二、项目结构

### git_training 仓库

```
git_training/
├── .gitignore       # 忽略编译产物，避免提交 build/ 和 *.o
├── Makefile         # 编译脚本
├── README.md        # 项目使用说明
├── calculator.h     # 四个运算函数的声明
├── calculator.cpp   # 四个运算函数的实现
└── main.cpp         # 程序入口，菜单与输入输出
```

各文件作用：

- `calculator.h`：声明 `add`、`sub`、`mul`、`divide` 四个函数
- `calculator.cpp`：四个函数的实现，除法对除数为 0 做了判断
- `main.cpp`：打印菜单、读取用户输入、根据选择调用对应函数
- `Makefile`：定义编译规则，支持 `make` 编译和 `make clean` 清理
- `.gitignore`：排除编译产物，防止 `*.o` 和可执行文件进入版本库

### HelloCV 仓库（本仓库）

```
HelloCV/
├── .gitignore        # 忽略编译产物
├── README.md         # 本文件，项目说明与使用方法
└── notes.md          # 语雀笔记链接汇总
```

## 三、环境配置步骤

### 1. 安装编译器

Linux（Ubuntu / Debian）：

```bash
sudo apt update
sudo apt install -y g++ make git
```

验证：

```bash
g++ --version
make --version
git --version
```

### 2. 配置 Git 全局身份

```bash
git config --global user.name "NoPoemEr"
git config --global user.email "1349843884@qq.com"
git config --global --list
```

### 3. 配置 SSH 免密访问

生成密钥：

```bash
ssh-keygen -t ed25519 -C "1349843884@qq.com"
```

查看公钥，并分别添加到 Gitee（设置 → SSH 公钥）和 GitHub
（Settings → SSH and GPG keys）：

```bash
cat ~/.ssh/id_ed25519.pub
```

验证：

```bash
ssh -T git@gitee.com      # 返回 Hi 用户名! 即成功
ssh -T git@github.com     # 返回 Hi 用户名! 即成功
```

### 4. 克隆 git_training 仓库

```bash
git clone git@gitee.com:Argentum_071/git_training.git
cd git_training
```

## 四、使用方法

### 编译

```bash
cd git_training
make
```

### 运行

```bash
./calculator
```

### 操作流程

程序启动后先打印菜单，输入 1 到 4 选择运算类型，再输入两个数，
回车后输出结果；输入 0 退出。

```
===== 简易计算器 =====
1. 加法
2. 减法
3. 乘法
4. 除法
0. 退出

请选择运算（0-4）：3
请输入两个数（用空格隔开）：6 7
计算：6 * 7
结果：42

请选择运算（0-4）：4
请输入两个数（用空格隔开）：7 2
计算：7 / 2
结果：3.5

请选择运算（0-4）：0
已退出
```

### 清理编译产物

```bash
make clean
```

## 五、Git 开发流程说明

`git_training` 的代码按功能拆分到两个分支开发：

1. 从 `master` 切出 `add-sub` 分支，完成加法和减法，提交后切回 `master`；
2. 从 `master` 切出 `mul-div` 分支，完成乘法和除法，提交后切回 `master`；
3. 两次使用 `git merge --no-ff` 把分支合并回 `master`，保留分支历史。

开发过程中使用的命令：`git init`、`git status`、`git add`、`git commit`、
`git branch`、`git switch`、`git merge`、`git stash`、`git stash pop`、
`git push`、`git pull`、`git log`。

完整过程、命令输出截图和每条命令的说明见语雀笔记，链接汇总在
[notes.md](notes.md)。

## 六、语雀笔记链接

所有笔记链接汇总见 [notes.md](notes.md)。

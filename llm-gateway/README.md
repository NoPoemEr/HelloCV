# LLM 决策网关：石头剪刀布

把一行 JSON 请求转发给 DeepSeek，再把模型返回的**任意文本**整理成一行**严格合法**的 JSON 决策。
坏输入、上游挂掉、模型胡说、网络抖动，都不允许让网关崩掉——永远一行进、一行出。

> 本目录基于官方训练仓库 `github.com/u5343/traning_repo` 的 `skeleton/` 实现：
> 10 个 TODO 全部自己实现，**没有拷贝 `example/`**。原任务书见 [TASK.md](TASK.md)。

## 协议

| | 内容 |
| --- | --- |
| 输入（每行） | `{"time": "<本轮时间>", "history": ["rock", "paper"]}`，`history` 可选 |
| 输出（每行） | `{"time": "<与输入一致>", "option": "rock\|scissor\|paper"}`，只允许这两个键 |
| 硬性不变式 | 一行进、一行出；任何异常都不影响输出，永远是合法 JSON |

胜负：`rock` 赢 `scissor`，`scissor` 赢 `paper`，`paper` 赢 `rock`。

## 数据流

```
一行 JSON
   │
   ├─ 输入非法 ──────────────► 本地兜底（当前时间 + rock），不花钱调上游
   │
   ▼
LlmClient.decide  ── 组装请求 / Bearer 鉴权 / 超时 / 重试
   │                    ├ 401·403 立刻失败 → 兜底
   │                    ├ 400     立刻失败 → 兜底
   │                    └ 超时·429·5xx·空回复 → 退避重试
   ▼
loads_object（三级容错）
   │   1. 直接 json.loads
   │   2. strip_code_fence：剥 ```json 围栏
   │   3. find_first_json_object：字符串感知地抠出配平的 {...}
   │   4. repair_json_text：尾逗号 / 单引号 / 注释 / 裸换行 / 缺括号
   │   5. 仍失败 → JsonGuardError → 兜底
   ▼
validate_decision  ── option 归一化；time 以请求为准；只放行两个键
   │
   ▼
出口自检 is_valid_decision ── 万一不合法 → rock
   │
   ▼
一行合法 JSON
```

## 目录结构

```
llm-gateway/
├── README.md            本文件：结构、环境、使用方法
├── TASK.md              官方任务书原文（评分标准）
├── requirements.txt     requests + pytest
├── gateway/
│   ├── config.py        配置（网关参数、API Key 只从环境变量读）
│   ├── prompts.py       系统提示词（json_object 模式要求提示词含 "json"）
│   ├── json_guard.py    ★ JSON 容错：脏文本 → dict
│   ├── rules.py         ★ 字段校验、枚举归一化、本地兜底
│   ├── client.py        ★ HTTP 转发：鉴权、重试、只取 content
│   ├── app.py           ★ 编排：一行进一行出，永不抛异常
│   └── game.py          对局与裁判
├── run_gateway.py       CLI：JSON 进 / JSON 出
├── play_game.py         和本地对手（或 DeepSeek）玩一局
├── mock_llm_server.py   本地假上游，故意返回各种坏数据
├── selfcheck.py         列出未实现的 TODO + 跑测试
├── samples/requests.jsonl  示例输入（含一行坏数据）
└── tests/               93 个验收测试
```

★ 标记的四个文件是本次实现的核心，其余是脚手架。

## 环境配置

要求 Python 3.8+（本机验证环境为 3.10.12）。

```bash
cd llm-gateway

# 建议用虚拟环境，避免污染系统 Python
python3 -m venv .venv
source .venv/bin/activate          # Windows: .venv\Scripts\activate

pip install -r requirements.txt

# API Key 只放环境变量，绝不写进代码或提交到仓库
export DEEPSEEK_API_KEY=sk-xxxxxxxx   # Windows PowerShell: $env:DEEPSEEK_API_KEY="sk-..."
```

可选参数通过 CLI 传入，不需要改代码：`--api-base-url`、`--model`、`--max-retries`、
`--request-timeout`、`--max-tokens`、`--enable-thinking`、`--offline`。

## 使用方法

**1）离线模式**（不调用 API，用本地规则出拳，先验证流水线）

```bash
echo '{"time": "2026-01-01T00:00:00+00:00"}' | python run_gateway.py --offline
# {"time":"2026-01-01T00:00:00+00:00","option":"rock"}
```

**2）对着本地假上游联调**（不花钱、可复现，假上游会依次返回围栏/废话/非法取值/空回复）

```bash
python mock_llm_server.py --port 8801 &
python run_gateway.py --api-base-url http://127.0.0.1:8801 --api-key test \
    --input samples/requests.jsonl
```

**3）真实 DeepSeek**

```bash
python run_gateway.py --input samples/requests.jsonl
python play_game.py --rounds 10
```

**4）文件进、文件出**

```bash
python run_gateway.py --input samples/requests.jsonl --output decisions.jsonl
```

每批处理结束会在 stderr 打一行统计：

```
处理 7 行：直接可用 3 / 已修正 1 / 本地兜底 3 / 输入非法 1
```

`已修正` = 模型答案能用但被网关改过（例如丢弃模型自报的 time）；
`本地兜底` = 上游/解析失败后由本地规则出拳。

## 设计要点（也是答辩要讲的三件事）

**1. 为什么容错要分三级？**
`json.loads` 是严格模式：模型返回 ```json 围栏、前后夹带自然语言、单引号、尾逗号、
`None`、少一个 `}`，它一律抛异常。所以按"从便宜到昂贵"逐级降级：剥围栏（纯字符串操作）
→ 括号配对（一次扫描，能跳过字符串里的 `}`）→ 文本修复（去注释、换字面量、补括号）。
每一级只做文本级、可解释的变换，**不用 `eval` / `ast.literal_eval`**——模型输出是不可信输入。

**2. 哪些错误重试，哪些立刻失败？**

| 情况 | 处理 | 原因 |
| --- | --- | --- |
| 401 / 403 | 立刻 `LlmAuthError` | key 错了，重试也不会变对 |
| 400 | 立刻 `LlmFatalError` | 模型名/参数不合法，重试浪费钱 |
| 408/409/425/429/5xx | 退避重试（尊重 `Retry-After`） | 服务端临时不可用 / 限流 |
| 网络异常（超时、连接重置） | 退避重试 | 抖动，重试大概率成功 |
| 200 但 `content` 为空 / 只给思维链 / `finish_reason=length` | 退避重试，耗尽后兜底 | 上游"答非所问"，换一次可能就好了 |

**3. 为什么 `time` 以请求为准？**
网关是可信边界。模型可能把 `time` 写错、写成自己的当前时间、甚至不带 `time`；
而下游是按请求里的 `time` 做对齐的。所以请求有 `time` 就用请求的（模型写的不一样就记录
"丢弃模型的 time"），请求没有才退而用模型的，两者都没有才用网关当前时间。

**兜底不是瞎猜**：有历史就出能赢对手上一手的拳（`counter_to`），没有历史就出 `rock`，
保证任何情况下都有一个合法、可解释的决策。

## 测试与验收

```bash
python selfcheck.py          # 列出未实现的 TODO，然后跑全部测试
python -m pytest tests -q    # 只跑测试
```

验收结果：**93 passed**，`selfcheck.py` 输出"所有 TODO 都实现完了"。

任务书要求的三条命令（离线 / 假上游 / 完整对局）均已验证，输出每行都是合法 JSON，
`option` 只在 `rock|scissor|paper` 三个取值内。

## 安全约定

- API Key 只从环境变量 `DEEPSEEK_API_KEY` 读取，代码里没有硬编码；
- `.env`、`decisions*.jsonl`、`__pycache__/` 已在 `.gitignore` 中忽略；
- 坏输入不转发给上游（既浪费钱，也可能把脏数据带进日志）。

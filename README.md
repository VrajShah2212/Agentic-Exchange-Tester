# 🚀 Agentic Exchange Stress-Tester

**Submission for the Microsoft Agents League Hackathon (Reasoning Agents Track)**

An autonomous, multi-step AI reasoning agent built to aggressively stress-test a custom C++ financial matching engine. 

Most AI projects wrap a language model around a text prompt. This project does the opposite: it forces an AI agent to interact with strict, low-level system constraints. By bridging Microsoft Foundry (via OpenAI standard clients) with a local C++ order book, this tool acts as an automated Quality Assurance tester for financial architectures.

---

## 🧠 The Architecture

The system is broken down into three frictionless layers:

1. **The C++ Matching Engine (Core Logic):** A robust, memory-efficient order book that processes `BUY` and `SELL` limits. It includes an embedded **Escrow & Wallet Management** system. If an order lacks sufficient USD or Crypto Token backing, the engine actively rejects the trade and throws an error flag.
2. **The Python Bridge (Observability):** A lightweight communication pipeline using `subprocess`. It launches the compiled C++ executable, streams live data via standard input/output, and completely bypasses the need for complex command-line environment configurations. 
3. **The Reasoning Agent (AI Brain):** The AI reads the real-time `BOOK_STATUS` and transaction logs. When the C++ engine blocks an illegal trade, the agent utilizes multi-step reasoning to recognize its liquidity failure, pivot its strategy, and submit corrected orders.

---

## ✨ Key Features

* **True Multi-Step Reasoning:** The AI doesn't just guess; it reads `ORDER_REJECTED` logs and adapts its trading behavior based on its simulated capital constraints.
* **Low-Latency Order Matching:** The C++ core clears partial fills, handles memory clean-up, and processes bid/ask spreads instantly.
* **Zero-Friction Execution:** Designed specifically to run without complex CLI arguments. Just compile and run the Python bridge.

---
<img width="916" height="847" alt="image" src="https://github.com/user-attachments/assets/acb8a84e-24a0-49d9-90dd-5f560076e1f8" />

## 🛠️ How to Run Locally

### Prerequisites
* A C++ compiler (e.g., MinGW `g++` or Clang)
* Python 3.x
* OpenAI API Key (or local LLM via Ollama)

### Setup Instructions

**1. Compile the Engine**
Navigate to the project folder and compile the C++ core:
`g++ engine.cpp -o engine.exe`
*(Note: On Mac/Linux, use `-o engine`)*

**2. Set Your Environment Variable**
Securely pass your API key to the terminal session:
* Windows: `$Env:OPENAI_API_KEY="your-api-key"`
* Mac/Linux: `export OPENAI_API_KEY="your-api-key"`

**3. Launch the AI Bridge**
Initiate the automated reasoning loop:
`python bridge.py`

Watch the terminal as the AI evaluates the market, attempts extreme trading vectors, gets blocked by the Escrow logic, and intelligently adapts its strategy!

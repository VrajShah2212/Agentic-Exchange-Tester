import subprocess
import time

print("Starting the Offline AI-to-C++ Escrow Bridge...")

# Launch the upgraded C++ matching engine
process = subprocess.Popen(
    ['engine.exe'], # Change to './engine' if on Mac/Linux
    stdin=subprocess.PIPE,
    stdout=subprocess.PIPE,
    text=True
)

startup_msg = process.stdout.readline().strip()
print(f"System Status: {startup_msg}")

# --- THE SMART MOCK AI ---
def mock_ai_agent(engine_logs, order_id):
    """
    This offline function simulates an AI reasoning loop.
    It reads the C++ engine logs and pivots its strategy if it gets rejected.
    """
    print(f"   [AI Brain] Analyzing logs: {engine_logs}")
    
    # If the C++ engine rejected us for not having enough cash...
    if "Reason:Insufficient_USD" in engine_logs:
        print("   [AI Thought] 'I am out of cash! I need to SELL tokens to raise capital.'")
        return f"{order_id} SELL 50000.00 50"
        
    # If the C++ engine rejected us for not having enough tokens...
    elif "Reason:Insufficient_Tokens" in engine_logs:
        print("   [AI Thought] 'I am out of tokens! I will place a small BUY order.'")
        return f"{order_id} BUY 49500.00 1"
        
    # Default Aggressive Move: Try to buy $5.1 Million worth of crypto!
    # (Since User 1 only has $100,000, this SHOULD trigger your C++ Escrow block)
    else:
        print("   [AI Thought] 'Market looks clear. Initiating massive $5.1M buy order to test liquidity.'")
        return f"{order_id} BUY 51000.00 100" 

# -------------------------

print("\n--- Initiating Offline Reasoning Loop ---")

current_book_state = startup_msg

for i in range(5): 
    print(f"\n[Iteration {i+1}]")
    
    # Ask the local mock AI what to do based on the C++ output
    generated_order = mock_ai_agent(current_book_state, i+1)
    print(f"   [Mock AI Action] Executing: '{generated_order}'")
    
    # Send to C++
    process.stdin.write(generated_order + "\n")
    process.stdin.flush() 
    
    # Read everything the C++ engine outputs
    engine_responses = []
    while True:
        line = process.stdout.readline().strip()
        engine_responses.append(line)
        if line.startswith("BOOK_STATUS"):
            break
    
    full_engine_feedback = ""
    for resp in engine_responses:
        print(f"   [C++ Engine Core] {resp}")
        full_engine_feedback += resp + " | "
        
    current_book_state = full_engine_feedback
    time.sleep(3) 

process.terminate()
print("\nOffline Integration Session Complete.")
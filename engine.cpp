#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <map>

// --- NEW WALLET STRUCTURE ---
struct Wallet {
    double usd;
    int tokens;
};

// We use a map to link a User ID to their specific Wallet balances
std::map<int, Wallet> wallets;

struct Order {
    int userId; 
    std::string type;
    double price;
    int qty;
};

std::vector<Order> buyOrders;
std::vector<Order> sellOrders;

// --- INITIALIZE STARTING BALANCES ---
void setupWallets() {
    // Give 10 simulated users $100,000 USD and 500 Crypto Tokens each to start
    for (int i = 1; i <= 10; ++i) {
        wallets[i] = {100000.0, 500};
    }
}

void matchOrders() {
    while (!buyOrders.empty() && !sellOrders.empty() && buyOrders[0].price >= sellOrders[0].price) {
        
        int tradeQty = std::min(buyOrders[0].qty, sellOrders[0].qty);
        double tradePrice = sellOrders[0].price; 

        int buyerId = buyOrders[0].userId;
        int sellerId = sellOrders[0].userId;

        // --- NEW SETTLEMENT LOGIC ---
        // 1. Give tokens to the buyer
        wallets[buyerId].tokens += tradeQty;
        
        // 2. Give USD cash to the seller
        wallets[sellerId].usd += tradePrice * tradeQty; 
        
        // 3. Refund the buyer if they placed a Buy order at a higher price than the trade matched at
        double spreadRefund = (buyOrders[0].price - tradePrice) * tradeQty;
        wallets[buyerId].usd += spreadRefund;

        std::cout << "TRADE_EXECUTED|Buyer:" << buyerId 
                  << "|Seller:" << sellerId 
                  << "|Price:" << tradePrice 
                  << "|Qty:" << tradeQty << std::endl;

        buyOrders[0].qty -= tradeQty;
        sellOrders[0].qty -= tradeQty;

        if (buyOrders[0].qty == 0) buyOrders.erase(buyOrders.begin());
        if (sellOrders[0].qty == 0) sellOrders.erase(sellOrders.begin());
    }
}

void addOrder(int userId, std::string type, double price, int qty) {
    // --- NEW PRE-TRADE VALIDATION (ESCROW) ---
    if (type == "BUY") {
        double totalCost = price * qty;
        if (wallets[userId].usd < totalCost) {
            std::cout << "ORDER_REJECTED|Reason:Insufficient_USD|UserID:" << userId << std::endl;
            return; // Stop the function, order fails
        }
        wallets[userId].usd -= totalCost; // Lock the funds in escrow
        
        buyOrders.push_back({userId, type, price, qty});
        std::sort(buyOrders.begin(), buyOrders.end(), [](Order a, Order b) { return a.price > b.price; });
        
    } else if (type == "SELL") {
        if (wallets[userId].tokens < qty) {
            std::cout << "ORDER_REJECTED|Reason:Insufficient_Tokens|UserID:" << userId << std::endl;
            return; // Stop the function, order fails
        }
        wallets[userId].tokens -= qty; // Lock the tokens in escrow
        
        sellOrders.push_back({userId, type, price, qty});
        std::sort(sellOrders.begin(), sellOrders.end(), [](Order a, Order b) { return a.price < b.price; });
    }
    
    matchOrders();
}

int main() {
    std::string type;
    int userId, qty;
    double price;

    setupWallets();
    std::cout << "ENGINE_READY" << std::endl;

    while (std::cin >> userId >> type >> price >> qty) {
        addOrder(userId, type, price, qty);
        
        std::cout << "BOOK_STATUS|TopBuy:" << (buyOrders.empty() ? 0 : buyOrders[0].price)
                  << "|TopSell:" << (sellOrders.empty() ? 0 : sellOrders[0].price) << std::endl;
    }
    return 0;
}
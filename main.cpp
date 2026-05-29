#include <iostream>
#include <chrono>
#include <random>
#include "ASCAR.hpp" // Pulls in your engine's blueprint safely

int main() {
    std::cout << "--- ASCAR 100-Million Order Benchmark ---\n";
    std::cout << "Initializing Engine...\n";

    // 1. Setup the Engine
    Bids emptyBids;
    Asks emptyAsks;
    OrderMap emptyMap;
    OrderBook book(emptyBids, emptyAsks, emptyMap);

    // 2. Setup High-Speed Random Number Generation (Hardware level)
    std::random_device rd;
    std::mt19937 gen(rd()); // Mersenne Twister engine for raw speed
    std::uniform_real_distribution<> priceDist(100.0, 150.0);
    std::uniform_int_distribution<Tquantity> qtyDist(1, 100);
    std::uniform_int_distribution<> sideDist(0, 1); // 0 for Asks, 1 for Bids

    const int TOTAL_ORDERS = 100000000;

    std::cout << "Firing 100,000,000 orders...\n";

    // 3. Start the Hardware Stopwatch
    auto start = std::chrono::high_resolution_clock::now();

    // 4. The Main Loop
    for (Torderid i = 1; i <= TOTAL_ORDERS; ++i) {
        // Generate random parameters
        Tprice randomPrice = std::round(priceDist(gen) * 100.0) / 100.0; // Round to 2 decimals
        Tquantity randomQty = qtyDist(gen);
        Sides randomSide = (sideDist(gen) == 0) ? Sides::Asks : Sides::Bids;

        // Allocate and insert the order
        Order* newOrder = new Order(i, randomPrice, randomQty, 0, randomSide);
        book.addOrder(newOrder);

        // Instantly attempt to match
        book.matchOrders();
    }

    // 5. Stop the Stopwatch
    auto end = std::chrono::high_resolution_clock::now();

    // Calculate duration
    std::chrono::duration<double, std::milli> elapsed = end - start;

    std::cout << "--- Benchmark Complete ---\n";
    std::cout << "Processed: " << TOTAL_ORDERS << " orders.\n";
    std::cout << "Time Elapsed: " << elapsed.count() << " ms.\n";
    std::cout << "Throughput: " << (TOTAL_ORDERS / (elapsed.count() / 1000.0)) << " orders/second.\n";

    return 0;
}

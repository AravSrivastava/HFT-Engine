#HFT-ENGINE

# High Frequency Trading (HFT) Engine

A low-latency High Frequency Trading (HFT) engine written in C++ that implements the foundational architecture of a modern electronic exchange order book.

This project focuses on:

- Order management
- Bid/Ask book handling
- Price level aggregation
- Order cancellation
- Efficient lookup structures
- Core exchange architecture concepts

---

# Table of Contents

- [Overview](#overview)
- [What is an HFT Engine?](#what-is-an-hft-engine)
- [Current Features](#current-features)
- [Project Architecture](#project-architecture)
- [Code Explanation](#code-explanation)
- [Data Structures Used](#data-structures-used)
- [Known Issues](#known-issues)
- [Future Improvements](#future-improvements)
- [How Orders Flow Through the Engine](#how-orders-flow-through-the-engine)
- [Compilation](#compilation)
- [Learning Outcomes](#learning-outcomes)
- [Roadmap](#roadmap)

---

# Overview

This project is the foundation of a High Frequency Trading matching engine.

A matching engine is the core component of every exchange system. It is responsible for:

- Receiving orders
- Organizing orders
- Matching buyers and sellers
- Executing trades

The engine currently implements the fundamental infrastructure required before actual order matching logic is added.

---

# What is an HFT Engine?

High Frequency Trading systems are designed for ultra-low latency trading where thousands or millions of orders are processed every second.

The matching engine is the heart of such systems.

Examples of real-world systems include:

- NASDAQ
- NYSE
- Binance
- CME Group

The primary goals of an HFT engine are:

- Speed
- Low latency
- Deterministic execution
- Efficient memory usage
- Fast order lookup
- Real-time order matching

---

# Current Features

## ✅ Order Representation

Each order contains:

- Order ID
- Price
- Quantity
- Timestamp
- Side (Bid/Ask)

---

## ✅ Price Level Management

Orders are grouped by price into price levels.

Example:

| Price | Orders |
|------|------|
| 101.0 | 5 |
| 100.5 | 3 |

This mirrors how real exchanges maintain market depth.

---

## ✅ Bid and Ask Separation

The order book maintains:

- Bid side (Buy orders)
- Ask side (Sell orders)

This allows efficient matching logic later.

---

## ✅ Order Lookup Table

Orders are stored inside an order map for quick retrieval.

Useful for:

- Order cancellation
- Order modification
- Fast searching

---

## ✅ Object-Oriented Design

The project is modularized into:

- `Order`
- `PriceLevel`
- `OrderBook`

This makes future expansion easier.

---

# Project Architecture

```text
                Incoming Orders
                       │
                       ▼
                ┌─────────────┐
                │ Order Book  │
                └──────┬──────┘
                       │
        ┌──────────────┴──────────────┐
        ▼                             ▼
   ┌──────────┐                 ┌──────────┐
   │   Bids   │                 │   Asks   │
   └──────────┘                 └──────────┘
        │                             │
        ▼                             ▼
   Price Levels                  Price Levels
        │                             │
        ▼                             ▼
      Orders                        Orders
```

---

# Code Explanation

# 1. Header Files

```cpp
#include <iostream>
#include <sys/_types/_u_int32_t.h>
#include <vector>
#include <map>
#include <algorithm>
```

## Purpose

| Header | Purpose |
|------|------|
| `<iostream>` | Console I/O |
| `<vector>` | Dynamic order storage |
| `<map>` | Sorted price levels |
| `<algorithm>` | STL algorithms |
| `<sys/_types/_u_int32_t.h>` | Fixed-size integer types |

---

# 2. Enum for Order Side

```cpp
enum class Sides{
    Asks,
    Bids
};
```

Defines whether an order is:

- Buy order (`Bids`)
- Sell order (`Asks`)

Using `enum class` improves:

- Type safety
- Readability
- Namespace protection

---

# 3. Type Aliases

```cpp
using Torderid  = u_int32_t;
using Tprice = double;
using Tquantity = u_int32_t;
using Ttimestamp = u_int64_t;
using TSide = Sides;
```

These aliases improve readability and maintainability.

Example:

```cpp
using Torderid = uint64_t;
```

Can later upgrade the engine to support larger order IDs.

---

# 4. Order Structure

```cpp
struct Order{
    Torderid OrderID;
    Tprice Price;
    Tquantity Quantity;
    Ttimestamp Timestamp;
    TSide Side;
```

Represents a single market order.

## Fields

| Field | Description |
|------|------|
| `OrderID` | Unique order identifier |
| `Price` | Order price |
| `Quantity` | Number of shares/contracts |
| `Timestamp` | Arrival time |
| `Side` | Bid or Ask |

---

# 5. Order Storage

```cpp
using Torders = std::vector<Order*>;
```

Orders are stored as pointers for performance reasons.

Benefits:

- Avoid object copying
- Faster insertion
- Lower memory overhead

Important in low-latency systems.

---

# 6. PriceLevel Class

```cpp
class PriceLevel{
```

Groups all orders at the same price.

Example:

```text
Price = 101.0
Orders = [O1, O2, O3]
```

---

## Internal Members

```cpp
Tprice Price;
Tquantity TotalQuantity;
Torders Orders;
```

| Member | Purpose |
|------|------|
| `Price` | Price represented by level |
| `TotalQuantity` | Total liquidity |
| `Orders` | Orders at this price |

---

## Adding Orders

```cpp
void addOrder(Order *order){
    Orders.emplace_back(order);
    TotalQuantity += order -> Quantity;
}
```

### What Happens?

1. Order added to vector
2. Quantity updated

---

## Removing Orders

```cpp
void removeOrder(Torderid OrderId)
```

Searches for the order and removes it.

---

## Front Order Access

```cpp
Order* front()
```

Returns the first order at the price level.

Used for:

- FIFO execution
- Price-time priority

---

# 7. Bid and Ask Containers

```cpp
using Bids = std::map<double, PriceLevel>;
using Asks = std::map<double, PriceLevel>;
```

## Why `std::map`?

Maps maintain sorted ordering automatically.

This is important because:

- Best Bid = highest bid price
- Best Ask = lowest ask price

---

# 8. Order Lookup Map

```cpp
using OrderMap = std:: map<Torderid, Order*>;
```

Provides fast access to orders using their ID.

Useful for:

- Cancel order
- Modify order
- Search order

---

# 9. OrderBook Class

```cpp
class OrderBook{
```

The central engine component.

Maintains:

- Bid book
- Ask book
- Global order tracking

---

## Internal Members

```cpp
Bids bids;
Asks asks;
OrderMap orderMap;
```

---

## Adding Orders

```cpp
void addOrder(Order* order)
```

### Flow

1. Store order in `orderMap`
2. Check order side
3. Insert into bid/ask book
4. Update quantity

---

## Cancelling Orders

```cpp
void cancelOrder(Torderid orderID)
```

Currently removes orders only from:

- `orderMap`

Future versions should also remove them from:

- Price levels
- Bid/Ask books

---

# Data Structures Used

| Structure | Purpose | Reason |
|------|------|------|
| `vector` | Order storage | Cache-friendly |
| `map` | Price levels | Sorted ordering |
| `map` | Order lookup | Fast retrieval |

---

# Known Issues

# 1. Quantity Removal Bug

Current code:

```cpp
TotalQuantity -= (*it) -> OrderID;
```

Correct version:

```cpp
TotalQuantity -= (*it) -> Quantity;
```

The engine should reduce quantity, not order ID.

---

# 2. Front Function Logic Bug

Current code:

```cpp
return (Orders.empty())?Orders[0]:nullptr;
```

Correct version:

```cpp
return (!Orders.empty()) ? Orders[0] : nullptr;
```

---

# 3. Missing Default Constructor

This line:

```cpp
bids[order->Price]
```

Requires a default constructor for `PriceLevel`.

Add:

```cpp
PriceLevel() : Price(0), TotalQuantity(0), Orders{} {}
```

---

# 4. Access Specifier Issue

Inside `OrderBook`, methods are private by default.

Need:

```cpp
public:
```

inside the class.

---

# How Orders Flow Through the Engine

## Example: Buy Order

```text
Buy 100 shares @ 101
```

Flow:

```text
Order Created
      ↓
Inserted into OrderMap
      ↓
Inserted into Bid Side
      ↓
Inserted into Price Level
      ↓
Total Quantity Updated
```

---

# Compilation

## Using g++

```bash
g++ -std=c++17 main.cpp -o hft_engine
```

Run:

```bash
./hft_engine
```

---

# Learning Outcomes

This project demonstrates concepts such as:

- Order book mechanics
- Trading system architecture
- Low-latency design
- Data structure optimization
- C++ OOP design
- Memory management
- Price-time priority concepts

---

# Future Improvements

## Matching Engine

Implement actual trade execution:

```text
Best Bid >= Best Ask
```

---

## Market Orders

Support:

- Market Buy
- Market Sell

---

## Partial Fills

Allow partial order execution.

---

## Advanced Order Types

Add support for:

- Stop orders
- Iceberg orders
- Fill-or-kill
- IOC orders

---

## Performance Optimizations

Possible improvements:

- `unordered_map`
- Memory pools
- Lock-free structures
- Custom allocators

---

## Multithreading

Future HFT systems may include:

- Market data thread
- Matching thread
- Risk management thread

---

## Networking

Potential additions:

- FIX protocol
- TCP market gateway
- WebSocket feeds

---

# Roadmap

## Planned Features

- [ ] Matching algorithm
- [ ] Trade execution
- [ ] Partial fills
- [ ] Market orders
- [ ] Risk management
- [ ] Logging
- [ ] Latency benchmarking
- [ ] Persistence layer
- [ ] Market data feed
- [ ] FIX protocol integration

---

# Conclusion

This project establishes the foundation of a modern High Frequency Trading engine.

Although still in early development, it already demonstrates:

- Efficient order organization
- Price-level aggregation
- Bid/Ask separation
- Fast order lookup architecture

The next phase will involve implementing the actual matching engine and optimizing the system for realistic low-latency trading workloads.
#HFT-ENGINE

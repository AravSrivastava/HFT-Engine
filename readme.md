# ASCAR: High-Frequency Limit Order Book (LOB) Engine

ASCAR is a custom-built, ultra-low latency limit order book and matching engine written entirely in modern C++. It is designed to simulate the core execution logic used by quantitative trading firms and cryptocurrency exchanges.

## 🚀 Performance Benchmark
* **Throughput:** 1.1 Million Orders / Second (Tested via a 100-million order chaotic market simulation).
* **Latency:** Sub-microsecond execution time per order.
* **Hardware Profile:** Benchmarked natively on an Apple Silicon M4 Pro.

## ⚙️ Core Architecture
The engine is built around deterministic algorithmic matching, prioritizing strict price-time priority execution. 

* **Price Level Tracking:** Implemented via standard associative containers (`std::map`) for log(N) price tier discovery.
* **Order Queue Management:** Utilizes constant time O(1) removals via doubly linked lists (`std::list`) to prevent array shifting penalties during aggressive order sweeps.
* **Memory Safety:** Features strict ownership rules and autonomous heap cleanup during partial and full order fills to prevent memory leaks during continuous uptime.

## 🛠️ Technical Stack
* **Language:** C++20 / C++23
* **Build Tools:** Clang++ with `-O3` aggressive optimization flags
* **Libraries:** `<map>`, `<list>`, `<chrono>` (for high-resolution hardware timers), `<random>` (Mersenne Twister PRNG for load generation).

## 🔮 Future Optimization Roadmap
While the current engine handles millions of operations per second, the next iteration is focused on overcoming standard library memory bottlenecks:
1. **Object Pooling:** Eradicating dynamic heap allocations (`new`/`delete`) during runtime.
2. **Cache Locality:** Migrating from node-based structures to contiguous memory buffers (Ring Buffers/Flat Arrays) to eliminate CPU L2/L3 cache misses.
3. **Direct Indexing:** Replacing Red-Black tree lookups with direct array indexing for O(1) price tier access.

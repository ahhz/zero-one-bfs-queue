The 01-BFS is used for shortest path analysis where the cost of a node or edge is 0 or 1. Typically this is implemented with a deque based priority_queue that places high priority at the front and high priority at the back. Here a solution with two buckets is used, which is a more efficient approach because if avoids the complications of a deque.

## Benchmark: two_tier_queue vs std::deque

This repository includes `benchmark_01_bfs.cpp` to compare:

- `two_tier_queue<int>`
- `std::deque<int>` (via a small adapter)

The benchmark is designed to be fair:

- Both implementations run on identical, pre-generated random 0-1 graphs.
- The execution order alternates every round to reduce thermal/turbo bias.
- Warmup rounds are excluded from timing.
- A checksum is validated every round to guarantee both queues produce identical BFS outputs.

### Build

GCC/Clang:

```bash
g++ -std=c++20 -O3 -DNDEBUG -march=native benchmark_01_bfs.cpp -o bench_01_bfs
```

MSVC (Developer Command Prompt):

```bat
cl /std:c++20 /O2 /DNDEBUG /EHsc benchmark_01_bfs.cpp /Febench_01_bfs.exe
```

### Run

```bash
./bench_01_bfs
```

Useful options:

```text
--cases N           Number of workloads
--base-nodes N      Nodes in first workload
--node-step N       Added nodes per workload
--edges-per-node N  Outgoing edges per node
--sources N         Source nodes per workload
--warmup N          Warmup rounds
--runs N            Timed rounds
--seed N            RNG seed
```

Example:

```bash
./bench_01_bfs --cases 4 --base-nodes 50000 --node-step 25000 --edges-per-node 8 --sources 12 --warmup 2 --runs 10 --seed 42
```

### Sample results

From a Windows/MSVC run with:

```text
.\bench_01_bfs.exe --cases 2 --base-nodes 20000 --node-step 10000 --edges-per-node 6 --sources 6 --warmup 1 --runs 3 --seed 42
```

```text
0-1 BFS benchmark (same workloads, alternating order, checksummed)
cases=2 base_nodes=20000 node_step=10000 edges_per_node=6 sources=6 warmup=1 runs=3 seed=42

workload                   deque(ms)      two_tier(ms)       speedup
--------------------------------------------------------------------
case_1_n20000                 13.014             8.758         1.49x
case_2_n30000                 20.664            14.007         1.48x
--------------------------------------------------------------------
overall(avg sum)              33.678            22.765         1.48x
```

In this run, `two_tier_queue` was about `1.48x` faster than `std::deque` on the selected workloads.

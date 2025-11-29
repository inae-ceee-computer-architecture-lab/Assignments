# 📘 Cache aware matrix multiplication

This project implements several optimized versions of square matrix multiplication in C/C++, measuring performance with high-resolution timers.
The goal is to compare:

1. **Naive O(N³) multiplication**
2. **Loop-reordered multiplication (i-k-j)**
3. **Cache-tiled / blocked multiplication**
4. **Combined tiling + loop optimization**


---

## 📂 Implemented Methods

### ### 1️⃣ Naive Matrix Multiplication

**Order:** `i → j → k`
**Characteristics:**

* Triple nested loop
* Very poor cache performance
* Baseline for performance comparison

---

### 2️⃣ Loop-Optimized Multiplication (i-k-j)

**Function:** `loop_opt_mat_mul`

Reorders loops to:

```
for i
  for k
    for j
```

### Why this is faster

* Accesses an entire **row of B** sequentially → better spatial locality
* Reuses `A[i][k]` in the innermost loop → stays in registers
* Reduces cache misses compared to naive `i-j-k` order

**Performance:** Typically **1.3×–2× faster** than naive.

---

### 3️⃣ Tiled / Blocked Matrix Multiplication

**Function:** `tile_mat_mul`

Breaks matrices into smaller **blocks / tiles** that fit in the L1/L2 CPU cache:

```
for ii in 0..N step block
  for kk in 0..N step block
    for jj in 0..N step block
      multiply small sub-blocks
```

### Why this is faster

* Reduces cache thrashing
* Operates on small submatrices that **stay in cache**
* Improves spatial & temporal locality
* Large performance boost for bigger matrices (>256×256)

---

### 4️⃣ Combined Tiling + Loop Optimization

**Function:** `combination_mat_mul`

Inside each tile, the loop order is the optimized `i-k-j`.

### Why this is fastest

* Loop order improves locality
* Tiling ensures reused values remain in cache
* Frequently the best-performing non-SIMD technique

---

## 🧱 Choosing the Tile Size (Block Size)

Tile size is **critical** to performance.

### ✔ General Guidelines

* Tile should be chosen so that **three tiles** (A block, B block, C block) fit into the **L1 or L2 cache**.
* Modern CPUs have:

  * L1: 32 KB per core
  * L2: 256 KB – 1 MB per core

### ✔ Recommended Tile Sizes

| Matrix Size | Recommended Tile Size |
| ----------- | --------------------- |
| 64–512      | 16, 32                |
| 512–2048    | 32, 64                |
| >2048       | 64, 128               |

### ✔ Rule of Thumb

To fit comfortably in L1 cache:

```
block_size ≈ sqrt( (L1_cache_size / 3) / sizeof(double) )
```

For a 32 KB L1 cache:

```
block_size ≈ sqrt( (32768 / 3) / 8 )
block_size ≈ sqrt(1365) ≈ 36 → Use 32
```

---
## ▶ Building & Running

### Build

Your Makefile likely contains targets for different optimizations:

```bash
make naive
make loop
make tiling
make combined
```

### Run

```bash
./mat_mul <matrix_size>
```

Example:

```bash
./mat_mul/tiling 1024
```

---

## 📊 Output Example

```
Naive matmul: 3245000 microseconds

Tiled matmul: 820000 microseconds
Speedup: 3.95
```

---

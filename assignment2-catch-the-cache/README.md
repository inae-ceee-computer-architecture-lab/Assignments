# Cache Size Prediction Assignment Documentation

## Overview

This assignment explores cache memory properties by developing a program that can predict cache sizes through performance analysis. The fundamental principle is that memory access time varies depending on whether data fits within different levels of the cache hierarchy.

## Objectives

1. Understand cache hierarchy and its impact on program performance
2. Develop a C++ program to measure memory access times for varying array sizes
3. Analyze timing data to infer cache sizes at different levels (L1, L2, L3/LLC)
4. Visualize results using plotting tools

## Background Theory

### Cache Hierarchy Behavior

The cache hierarchy consists of multiple levels with different characteristics: (on some PC do not have L3 cache)

- **L1 Cache:** Smallest, fastest, closest to CPU core
- **L2 Cache:** Larger than L1, slightly slower
- **L3 Cache (LLC):** Largest, slowest cache level, often shared across cores

### Access Time Principle

Memory access time depends on cache hit/miss patterns:

**Small Array (fits in L1):**

- Entire array loads into L1 cache
- Subsequent accesses are L1 hits
- Result: Very low average access time

**Large Array (exceeds L1):**

- Only portion fits in L1 cache
- Many accesses result in L1 misses → L2/L3 lookups
- Result: Higher average access time

**Transition Points:**

- Sharp increases in access time occur when array size exceeds cache capacity
- These transition points reveal cache sizes

## Implementation Requirements

### Program Specifications

**File:** `cache-size-pred.cpp`

**Functionality:**

1. Create arrays of varying sizes
2. Access array elements repeatedly
3. Measure average access time per element
4. Output results in specified format

### Key Implementation Considerations

#### 1. Cache Line Awareness

**Critical:** Each access must target a new cache line to accurately sample cache behavior.

**Reason:** Spatially local accesses (within same cache line) will be L1 hits, diluting measurements and increasing noise.

**Implementation:** Stride through the array with step size equal to cache line size (typically 64 bytes).

```cpp
// Example: Access every cache line (64 bytes apart)
for (size_t i = 0; i < array_size; i += CACHE_LINE_SIZE) {
    // Access array[i]
}
```

#### 2. Per-Core Cache Sizing

When interpreting `lscpu` output, calculate per-core cache size:

```
Per-core size = Total cache size / Number of instances
```

**Example:**

```
L1d: 768 KiB (12 instances)
Per-core L1d = 768 KB / 12 = 64 KB
```

#### 3. Statistical Accuracy

Take multiple measurements (recommended: ~100 iterations) and average them to:

- Reduce measurement noise
- Account for system variability
- Obtain reliable timing data

#### 4. Timing Strategy

**Do NOT:** Time individual memory accesses  
**DO:** Measure time for accessing entire buffer or large chunks

```cpp
// Correct approach
start_time = get_time();
for (int iter = 0; iter < NUM_ITERATIONS; iter++) {
    // Access entire array
}
end_time = get_time();
average_time = (end_time - start_time) / NUM_ITERATIONS / array_elements;
```

Reference `time_access.cpp` for timing implementation examples.

#### 5. Compiler Optimization

**Critical:** Compile with `-O0` flag to disable optimizations

**Reason:** Compiler optimizations may:

- Eliminate loops deemed "unnecessary"
- Reorder memory accesses
- Cache values in registers
- Produce misleading timing results

```bash
g++ cache-size-pred.cpp -o cache-size-pred -O0
```

#### 6. Measurement Environment

**Before running experiments:**

- Close background applications
- Minimize system processes
- Reduce external interference sources

**Purpose:** Prevent noise from context switches, interrupts, and competing processes

## Workflow

### Step 1: System Analysis

Examine your system's cache configuration:

```bash
lscpu
```

**Record:**

- L1d cache size (per core)
- L2 cache size (per core)
- L3/LLC cache size (total or per core)
- Cache line size

### Step 2: Program Development

Create `cache-size-pred.cpp` with the following logic:

```cpp
// Pseudocode structure
for each array_size from small to large:
    allocate array of array_size
    
    start_timer()
    for iteration in 1 to NUM_ITERATIONS:
        for i in 0 to array_size step CACHE_LINE_SIZE:
            access array[i]
    end_timer()
    
    calculate average_access_time
    print array_size, average_access_time
```

### Step 3: Data Collection

Compile and run the program, redirecting output:

```bash
g++ cache-size-pred.cpp -o cache-size-pred -O0
./cache-size-pred > data.txt
```

### Step 4: Visualization

Plot the collected data:

```bash
python3 plotter.py --filename data.txt
```

### Step 5: Analysis

Examine the plot to identify:

- Sharp increases in access time (cache size boundaries)
- L1 cache size: First significant jump
- L2 cache size: Second significant jump
- L3 cache size: Third significant jump (if present)

## Output Format

### Data File Structure

Follow the format specified in `format.txt` to ensure compatibility with the provided plotter.

**Typical format:**

```
array_size_1 average_time_1
array_size_2 average_time_2
array_size_3 average_time_3
...
```

**Units:**

- Array size: bytes or kilobytes
- Average time: nanoseconds or cycles

### Alternative Format

If modifying the plotter, you may use alternative formats such as CSV:

```csv
array_size,average_access_time
1024,2.5
2048,2.6
...
```

## Expected Results

### Typical Access Time Pattern

```
Array Size (KB)    Avg Access Time (ns)
1                  1.2
2                  1.2
4                  1.3
8                  1.2
16                 1.3
32                 1.2
64                 1.3  ← L1 boundary
128                2.8  ← L1→L2 transition
256                2.9
512                3.1
1024               3.0
2048               3.2  ← L2 boundary
4096               8.5  ← L2→L3 transition
8192               9.2
16384              9.5
```

### Graph Interpretation

**Expected plot characteristics:**

- Flat regions: Array fits entirely in current cache level
- Sharp steps: Array exceeds cache capacity, spills to next level
- Gradual increases: Partial cache utilization

## Troubleshooting

### Issue: No Clear Steps in Data

**Possible causes:**

- Not accessing new cache lines (fix stride)
- Too few iterations (increase averaging)
- Background interference (close processes)
- Compiler optimization (verify `-O0` flag)

### Issue: Noisy Measurements

**Solutions:**

- Increase number of iterations
- Run on isolated system
- Use larger array size increments
- Filter outliers in post-processing

### Issue: Unexpected Cache Sizes

**Check:**

- Per-core vs. total cache calculation
- Shared vs. private cache levels
- Inclusive vs. exclusive cache hierarchy

## Deliverables

1. **Source Code:** `cache-size-pred.cpp`
2. **Data File:** `data.txt` (program output)
3. **Plot:** Generated graph showing access time vs. array size
4. **Analysis:** Identified cache sizes with justification

## Grading Criteria

- Correct implementation of cache-aware access pattern
- Proper timing methodology
- Appropriate data collection range
- Clear visualization
- Accurate cache size predictions
- Understanding of cache hierarchy principles

## Additional Resources

### Reference Files Provided

- `time_access.cpp` - Example timing implementation
- `format.txt` - Required output format specification
- `plotter.py` - Python plotting script

### Compilation Commands

```bash
# Compile without optimization
g++ cache-size-pred.cpp -o cache-size-pred -O0

# Compile with debug symbols (optional)
g++ cache-size-pred.cpp -o cache-size-pred -O0 -g

# Check generated assembly (advanced)
g++ cache-size-pred.cpp -S -O0 -o cache-size-pred.s
```

### Python Plotter Usage

```bash
# Basic usage
python3 plotter.py --filename data.txt

# With custom output
python3 plotter.py --filename data.txt --output cache_analysis.png
```

## Learning Outcomes

Upon completion, you will understand:

1. Cache hierarchy organization and sizing
2. Performance impact of cache hits vs. misses
3. Memory access pattern optimization
4. Empirical performance measurement techniques
5. Data-driven system analysis
6. Cache-aware algorithm design principles

## Tips for Success

1. **Start small:** Test with limited array sizes first
2. **Verify assumptions:** Check actual cache sizes with `lscpu`
3. **Iterate gradually:** Use logarithmic or power-of-2 size increments
4. **Document findings:** Note unusual patterns or anomalies
5. **Cross-reference:** Compare predictions with `lscpu` output

## Advanced Considerations

### Cache Associativity

Cache replacement policies and set-associativity can affect results. Consider testing with:

- Sequential access patterns
- Strided access patterns
- Random access patterns

### False Sharing

Be aware of cache line contention in multi-threaded scenarios (not required for this assignment).

### Prefetching

Hardware prefetchers may mask some cache effects. Sequential access patterns may show less dramatic transitions than expected.

---


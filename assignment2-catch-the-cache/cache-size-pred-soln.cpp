#include <bits/stdc++.h>
#include <x86intrin.h> // for __rdtsc
using namespace std;

#define CACHE_LINESIZE 64

// Measure average access time (cycles) for a given array size
double measure_time_rdtsc(size_t array_size_bytes, int trials) {
    size_t n = array_size_bytes / sizeof(int);
    vector<int> arr(n);
    iota(arr.begin(), arr.end(), 0);

    // Warm-up: touch every element
    volatile int sink = 0;
    for (size_t i = 0; i < n; i += CACHE_LINESIZE / sizeof(int))
        sink += arr[i];

    // Measure using rdtsc with serialization
    uint64_t total_cycles = 0;
    for (int t = 0; t < trials; t++) {
        uint32_t cycles_high, cycles_low;
        asm volatile (
            "cpuid\n"       // serialize before
            "rdtsc\n"
            : "=a"(cycles_low), "=d"(cycles_high)
            :: "rbx","rcx"
        );
        uint64_t start = ((uint64_t)cycles_high << 32) | cycles_low;

        for (size_t i = 0; i < n; i += CACHE_LINESIZE / sizeof(int))
            sink += arr[i];

        asm volatile (
            "rdtscp\n"      // serialize after
            "mov %%edx, %0\n"
            "mov %%eax, %1\n"
            "cpuid\n"
            : "=r"(cycles_high), "=r"(cycles_low)
            :: "rax","rbx","rcx","rdx"
        );
        uint64_t end = ((uint64_t)cycles_high << 32) | cycles_low;
        total_cycles += (end - start);
    }

    double accesses = double(trials) * (double)n / (CACHE_LINESIZE / sizeof(int));
    return double(total_cycles) / accesses; // cycles per access
}

int main() {
    cout << "Estimating cache sizes (cycles per access)...\n";
    cout <<  "Size (KB)" << " Cycles/access\n";
    cout << "---------------------------\n";

    vector<size_t> sizes_kb;
    for (int kb = 1; kb <= 24000; kb = min(kb*2, kb+1024))
        sizes_kb.push_back(kb);

    for (auto kb : sizes_kb) {
        double t = measure_time_rdtsc(kb*1024, 1000);
        cout << kb  << " "<< t << "\n";
    }
}

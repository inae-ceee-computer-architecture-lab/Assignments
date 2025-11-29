#include <bits/stdc++.h>
#include <x86intrin.h>

//Demo function to show use of rdtsc
double measure_time_rdtsc(int trials) {

    int * dummy=(int*)malloc(sizeof(int));
    *dummy = 0;

    // Measure using rdtsc with serialization
    uint64_t total_cycles = 0;
    for (int t = 0; t < trials; t++) {
        uint32_t cycles_high, cycles_low;
        asm volatile (
            "cpuid\n"      
            "rdtsc\n"
            : "=a"(cycles_low), "=d"(cycles_high)
            :: "rbx","rcx"
        );
        uint64_t start = ((uint64_t)cycles_high << 32) | cycles_low;

        /*
        Put whatever code you want to measure execution time of here
        Dummy memory access given for reference
        */

       
        *dummy = *dummy + 1;
        *dummy = *dummy - 1;

        asm volatile (
            "rdtscp\n"      
            "mov %%edx, %0\n"
            "mov %%eax, %1\n"
            "cpuid\n"
            : "=r"(cycles_high), "=r"(cycles_low)
            :: "rax","rbx","rcx","rdx"
        );
        uint64_t end = ((uint64_t)cycles_high << 32) | cycles_low;
        total_cycles += (end - start);
    }

    return double(total_cycles) / trials; // cycles per access
}


int main(){
    std::cout << "Avg time taken for memory access averaged over 10 trials " << measure_time_rdtsc(10) << " cycles" <<  std::endl;
}
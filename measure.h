#ifndef _TIMING_H_
#define _TIMING_H_

#include <stdint.h>
#include <unistd.h>
#include <assert.h>
#include <map>
#include <vector>
#include <list>

typedef uint64_t pointer;

typedef std::pair<pointer, pointer> addrpair;


static inline __attribute__ ((always_inline))
void mfence()
{
	asm volatile ("mfence":::"memory");
}


static inline __attribute__ ((always_inline))
void lfence()
{
	asm volatile ("lfence":::"memory");
}

long utime() {
    struct timeval tv;
    gettimeofday(&tv, NULL);

    return (tv.tv_sec) * 1000 + (tv.tv_usec) / 1000;
}





static inline __attribute__ ((always_inline))
void cpuid()
{
	asm volatile ("cpuid":::"rax", "rbx", "rcx", "rdx");
}




extern inline __attribute__((always_inline))
uint64_t rdtsc() {
    uint64_t a, d;
    asm volatile ("xor %%rax, %%rax\n" "cpuid"::: "rax", "rbx", "rcx", "rdx");
    asm volatile ("rdtscp" : "=a" (a), "=d" (d) : : "rcx");
    a = (d << 32) | a;
    return a;
}

// ----------------------------------------------
extern inline __attribute__((always_inline))
uint64_t rdtsc2() {
    uint64_t a, d;
    asm volatile ("rdtscp" : "=a" (a), "=d" (d) : : "rcx");
    asm volatile ("cpuid"::: "rax", "rbx", "rcx", "rdx");
    a = (d << 32) | a;
    return a;
}



static inline __attribute__ ((always_inline))
uint64_t rdtscp(void)
{
	uint64_t lo, hi;
	asm volatile ("rdtscp\n":"=a" (lo), "=d"(hi)
		      ::"%rcx");
	return (hi << 32) | lo;
}

#endif

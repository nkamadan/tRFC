#include <bitset>
#include <iostream>
#include <stdio.h>
#include <assert.h>
#include <fcntl.h>
#include <linux/kernel-page-flags.h>
#include <stdint.h>
#include <sys/sysinfo.h>
#include <sys/mman.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <map>
#include <list>
#include <utility>
#include <fstream>
#include <set>
#include <algorithm>
#include <sys/time.h>
#include <sys/resource.h>
#include <sstream>
#include <iterator>
#include <math.h>

#include "measure.h"


struct histogram_vals{

    uint64_t start_cc;
    uint64_t latency;
};  

uint64_t no_of_accesses = 300000;
histogram_vals * histogram;


// ----------------------------------------------
void getTiming(pointer first, pointer second, uint64_t * timings, histogram_vals * histogram) {
   

//uint64_t timings[20]; //index 0 = 0-100 cycle, index 4 = 400-500 cycle ...	

	volatile size_t *f = (volatile size_t *) first;	
	volatile size_t *s = (volatile size_t *) second;
    //printf("cycles: %lld\n", cycles);
    uint64_t * lat = (uint64_t*)malloc(sizeof(uint64_t)*no_of_accesses);
    uint64_t init_cycles = rdtsc();
    uint64_t base_to_cut = (init_cycles/10000000000ULL) * 10000000000ULL;
    asm volatile("clflush (%0)" : : "r" (f) : "memory");
    for (uint64_t i = 0; i < no_of_accesses; i++) {
 
            *f;


        asm volatile("clflush (%0)" : : "r" (f) : "memory");
        lat[i] = rdtsc();
        
    }

            for(int i=0; i<no_of_accesses; i++){
                uint64_t res;
                if(i==0)
                    res = lat[i] - init_cycles;
                else
                    res = lat[i] - lat[i-1];
                uint64_t idx = res/100;
                timings[idx] += 1;
                histogram[i].start_cc = lat[i] - base_to_cut;
                histogram[i].latency = res;
            }
 
}

void clear_histogram(histogram_vals * hist, histogram_vals * new_hist){


    for(int i=0; i<no_of_accesses; i++){
        if(hist[i].latency > 630 && hist[i].latency < 1200){
            new_hist[i].start_cc = hist[i].start_cc;
            new_hist[i].latency = 950;
        }
        else if(hist[i].latency > 1200){ //anomaly
            new_hist[i].start_cc = hist[i].start_cc;
            new_hist[i].latency = 300;
        }
        else if(hist[i].latency > 330 && hist[i].latency < 630){  
            new_hist[i].start_cc = hist[i].start_cc;
            new_hist[i].latency = 300;
        }
        else{
            new_hist[i].start_cc = hist[i].start_cc;
            new_hist[i].latency = hist[i].latency; 
        }
    }

}





int main(){


setpriority(PRIO_PROCESS, 0, -20);

uint64_t a = 5;
uint64_t b = 6;
uint64_t * timings = (uint64_t*)malloc(sizeof(uint64_t)*20);
memset(timings, 0, sizeof(uint64_t)*20);
histogram_vals* histogram = (histogram_vals*)malloc(sizeof(histogram_vals)*no_of_accesses);

long start = utime();
getTiming((uint64_t)&a,(uint64_t)&b, timings, histogram);
long end = utime();

printf("accessing 300000 times took %ld ms\n", end - start);
for(int x=0; x<20; x++){
	printf("Range: %ld--%ld --> %ld\n",x*100, (x*100) + 100, timings[x]);

}

std::ofstream myfile;
myfile.open ("histogram-raw.txt");
for(int i=0; i<no_of_accesses; i++){
    myfile << histogram[i].start_cc << " " << histogram[i].latency << "\n";
}

myfile.close();

histogram_vals* new_histogram;
new_histogram = (histogram_vals*)malloc(sizeof(histogram_vals)*no_of_accesses);
clear_histogram(histogram, new_histogram);

myfile.open ("histogram.txt");
for(int i=0; i<no_of_accesses; i++){
    myfile << new_histogram[i].start_cc << " " << new_histogram[i].latency << "\n";
}

myfile.close();


return 0;
}





# tRFC

## Detect regular refreshes that memory controller issues every (7.8 us) - in most systems, on normal workloads. 

* Check the image 
![Refresh spikes](spikes.png) 

* Looking at the first spike, it occured at around ~647,000 (clock cycles). Looking at the second spike, it occurred at around ~662,000 (clock cycles). ~662,000 - ~647,000 = ~15,000. Frequency of the core that this code ran is set to 2.0 GhZ. ~15,000 cc / 2 = ~7,500 ns = ~7,5 us. 

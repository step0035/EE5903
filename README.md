# Power Aware Scheduling Algorithms

## 1. Blocking Aware Two Speeds

### Events
  1. Task Arrival
  2. Task Finished
  3. Task Blocked
  4. Resource Accessed
  5. Resource Freed

Before tasks starts executing, check for blocking potential
  
### Set fixed execution time for each CPU speed, relative to the WCET which is based on the lowest CPU speed
- Assume that one cycle is required to execute one instruction, thus, CPI = 1
- Using the equation
  
  ```
  CPU Execution Time = Instruction Count * CPI * Clock Cycle Time
                     = Instruction Count * (1 / Clock Speed)
  ```


Speed | Normalized Speed | Power Consumption | Exectution Time |
--- | --- | --- | --- |
624 MHz | 1.00 | 925 mW  | - |
520 MHz | 0.83 | 747 mW  | - |
416 MHz | 0.67 | 570 mW  | - |
312 MHz | 0.50 | 390 mW  | - |
208 MHz | 0.33 | 279 mW  | - |
104 MHz | 0.17 | 116 mW  | - |
Idle    | -    | 15.4 mW | - |
 

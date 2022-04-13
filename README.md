# Power Aware Scheduling Algorithms

## Prerequisites

It is recommended to run this simulation on Ubuntu or other Linux distributions

Install required packages
```
sudo apt update
sudo apt install build-essential make
```


## Building

In this project root directory, run
```
make
```
Two binaries, `bats_sim` and `ftpart_sim`, will be outputted in the project root directory

They are the BATS and FTPART algorithms respectively

You may also choose to build them separately
```
make bats
make ftpart
```


## Configurations

To configure the simulation parameters, edit the respective config files for each algorithms

Ensure that CONFIG_XXX_LOW is always lower or equal to CONFIG_XXX_HIGH

BATS   => `bats/inc/config.h`

FTPART => `ftpart/inc/config.h`


## Running the Simulation

To run the simulation
```
./bats_sim
./ftpart_sim
```


## References
* J. Wu, "BATS: An Energy-Efficient Approach to Real-Time Scheduling and Synchronization," 2014 IEEE Intl Conf on High Performance Computing and Communications, 2014 IEEE 6th Intl Symp on Cyberspace Safety and Security, 2014 IEEE 11th Intl Conf on Embedded Software and System (HPCC,CSS,ICESS), 2014, pp. 661-668, doi: 10.1109/HPCC.2014.96
* P. Zhu, D. Luo and X. Chen, "Fault-tolerant and power-aware scheduling in embedded real-time systems," 2020 International Conference on Computer, Information and Telecommunication Systems (CITS), 2020, pp. 1-5, doi: 10.1109/CITS49457.2020.9232471

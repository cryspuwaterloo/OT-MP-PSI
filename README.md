# Over-Threshold Multi-Party Private Set Intersection

This is experimental code for the paper "Practical Over-Threshold Multi-Party Private Set Intersection". 
It includes a C++ implementation of both of our Over-Threshold Multi-Party Private Set Intersection (OT-MP-PSI) protocols, t-PSI_0 (described in Section 3.3 of the paper) and t-PSI (Section 3.4). The implementation provides separate benchmarks for the share generation and reconstruction phases. In the share generation phase, we consider a single remote key-holder and simultaneously compute the shares for all parties. The reconstruction is carried out locally and is parallelized using OpenMP. After setting all parameters as described below, random datasets for the participants are generated and the intersection is computed. The code outputs verbose logs about the time taken for each operation. 

## Contents

- Over-Threshold Multi-Party Private Set Intersection
  - Contents
  - Dependencies
  - Building the project
    - Build Locally
    - Build from Docker
      * Run an image from Dockerhub
      * Build and run image from github using docker
  - Benchmarking
    - Examples
    - Protocols
      * Share Generation
      * Reconstruction
      * End-to-end
    - Experiments

## Dependencies
Experiments are to be run on a Unix machine. Also, the following dependencies are needed:

* [GMP](https://gmplib.org/)==6.1.2
* [NTL](https://www.shoup.net/ntl/)==11.4.1
* Paillier library: [libhcs](https://github.com/tiehuis/libhcs)

## Building the project

### Build locally

The dependencies can be installed individually from the links provided above. The ```install.sh``` script can be used to automate the installation of the dependencies.
Then run the following command to set the environment variables:
```export LD_LIBRARY_PATH="/usr/local/lib"```
Finally, run the ```make``` command in both the ```server``` and ```benchmark``` directories.

Note that both the script and the makefiles rely on global installation of the dependencies which might override existing libraries so  use this option with caution. We present alternative options below. 

### Build from docker
As a preliminary step, install docker from https://docs.docker.com/get-docker/ based on the OS you are running.
Two options are available for running the code using docker, pulling the pre-built image from dockerhub, or building the image from scratch.

**NOTE:** Getting the image from dockerhub is recommended, since it is much faster as opposed to building from scratch from github.

#### Run an image from Dockerhub

Pull the image from the repo

```docker pull johnbon7/ot-mpsi:1.0```

Then execute the appropriate commands below based on your OS to run the containers:

##### Windows/Mac
To get the image running as a container in the background:

```docker run --name <container_name> -it johnbon7/ot-mpsi:1.0 ```

Where <container_name> is the name of the container that you have chosen, for example "mycontainer1". 
You can open multiple CLIs for the image from the docker dashboard in docker desktop if you are using Windows or Mac.

##### Linux:
To get the image running as a container in the background:

 ```docker run --name <container_name> -d -it johnbon7/ot-mpsi:1.0```
 
Where <container_name> is the name of the container that you have chosen, for example "mycontainer1".
 
Use ```docker ps``` to find the container id of your container.

Use ```docker exec -it <container_id> bash``` to open a CLI to the container

Open another terminal and use ```docker exec -it <container_id> bash``` again to open a second CLI 

See https://docs.docker.com/engine/reference/commandline/run/ for additional info

 
#### Build and run image from github using docker

Clone the repo 

```git clone https://github.com/cryspuwaterloo/OT-MP-PSI.git```

Change directory to OT-MP-PSI and then run the following commands

Build the image:

```docker build --tag ot-mpsi:1.0 .```

Then execute the appropriate commands below based on your OS to run the containers:
##### Windows/Mac:

```docker run --name <container_name> -it ot-mpsi:1.0 ```

Where <container_name> is the name of the container that you have chosen, for example "mycontainer1". 
You can open multiple CLIs for the image from the docker dashboard in docker desktop if you are using Windows or Mac.

##### Linux:
Get the image running as a container in the background:

 ```docker run --name <container_name> -d -it ot-mpsi:1.0```
 
Where <container_name> is the name of the container that you have chosen, for example "mycontainer1". 
 
Use ```docker ps``` to find the container id of your container.

Use ```docker exec -it <container_id> bash``` to open a CLI to the container

Open another terminal and use ```docker exec -it <container_id> bash``` again to open a second CLI 

See https://docs.docker.com/engine/reference/commandline/run/ for additional info


## Benchmarking

**Keyholder Server:** As will be specified below, a keyholder server needs to be running for some of the benchmarks. This can be done by executing ```./server``` in the ```server``` directory.

**Benchmark Executable:** A command line interface has been provided to run the protocol with arbitrary parameters. To access this CLI execute ```./benchmark``` in the ```benchmark``` directory. 

#### Commands:
	sharegen: run a single share generation process, micro-benchmark
	recon: run the reconstruction process in only one bin, micro-benchmark
	all: run the end to end protocol, generating all shares, reconstructing in all bins

#### Options:
	-h	Show this help message
	-m	Number of parties (default=10)
	-n	Max number of elements (default=10)
	-t	Threshold (default=2)
	-b	Prime bit length (default=2048)
	-c	Constant for number of bins (comm. vs. comp. tradeoff) (default=1)
	-f	Regenerate elements for benchmarking instance (default=false)
	-k	IP Address of keyholder server (default 127.0.0.1)
	-r	Number of times to repeat experiment (has defaults)
	-s	Choice of Scheme 0=both,1,2 (default 0) 
	-l	Log results in file (default false) 
	-x	Don't run the reconstruction in the "all" command 
	-y	Fast Share Generation in the "all" command, for generating shares fast, without the actual protocol


### Examples
```./benchmark sharegen -t 2 -b 2048 -k 127.0.0.1 -r 1 -s 0``` <br/>
```./benchmark recon -m 10 -n 100 -t 2 -b 2048 -r 1 -s 0```<br/>
```./benchmark all -m 10 -n 100 -t 2 -b 2048 -k 127.0.0.1 -s 0``` <br/>

### Protocols
There are two main phases in the benchmark and in the protocol in general, namely the share generation and the reconstruction. Also to demostrate the end to end functionality, the full protocol can be run as well.

#### Share Generation
For the benchmarking the share generation (using the scheme specified by the -s option), the main parameters that are given as options are as follows:
* Threshold for the set intersection (t)
* Prime bit length (which specifies the bit security)

With these parameters, the share generation is executed for random elements (for as many times as specified by the -r option in the command) in communication with a keyholder server (at an address specified with the -k option). 
Mind that the number of elements (n) and the number of parties (m) are irrelevant in the time complexity of a single share generation.

**Output:** The average time for share generation is reported in the output. A more detailed report including average, min, max, and standard deviation of runtime and communication during different rounds of the protocol can be written to file in json format (if specified by the -l option)

Logs are all put in a directory with the following format based on the parameters:

```benchmark_recon_[t][prime bitsize]```

#### Reconstruction
Similarly, the input parameters of the reconstruction phase of the benchmark (using the scheme specified by the -s option), similar to the inputs of the actual protocol, are as follows:
* Number of parties (m)
* Max number of elements for each party (n)
* Threshold for the set intersection (t)
* Prime bit length (which specifies the bit security)
* Constant for Communication/Computation tradeoff (c)

For this benckmark, to measure the runtime, random numbers are generated as shares in a single bin (analogous to shares that are provided by the parties in a real scenario) and reconstruction is done with these shares. Reconstruction is done locally (for as many times as specified by the -r option).

**Output:** The average time for reconstruction in a single bin is reported in the output. A more detailed report including average, min, max, and standard deviation of runtime can be written to file in json format (if specified by the -l option)

Logs are all put in a directory with the following format based on the parameters:

```benchmark_recon_[m][n][t][prime bitsize]_[c]```

#### End to End Protocol
An end-to-end execution of the protocol is also available, by specifying all the parameters mentioned in the previous two protocols.
With the specified parameters, elements are generated for the parties and written to file, share generation is performed for all the parties in communication with the keyholder server and the shares are written to file, and finally the reconstruction is performed locally in all the bins.

**Output:** The average time for share generation and reconstruction is reported in the output.

Elements, shares, and logs are all put in a directory with the following format based on the parameters:

```benchmark_[m][n][t][prime bitsize]```

### Experiments
The two scripts in the benchmark directory, namely ```shargen_test.sh``` and ```recon_benchmark.sh```, are used to generate the results presented in the paper, for the share generation and reconstruction respectively.

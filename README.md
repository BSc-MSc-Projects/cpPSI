# cpPSI
Implementation of a Private Set Intersection scheme, using Microsoft SEAL C++ homomorphic cryptography library.

## Prerequisites
- cmake v 3.23
- Microsoft SEAL library v 4.0 (see [SEAL github repo](https://github.com/Microsoft/SEAL) to compile and install)

## Code organization
All the code is contained in `src` directory, where the logic for sender and receiver are respectively in `src/lib/sender.cpp` and `src/lib/receiver.cpp` files. There is also a utility file (`src/lib/receiver.cpp`) which contains functions used by both parties, and a test file (`src/test/test.cpp`) to verify that the scheme is working properly. 

## Compile and install
To compile and install this project, there is a CMakeLists.txt file so simply `cd` into `cpPSI` directory, then type inside a terminal
```
cmake .
make
```
This will generate the `test` binary file, which contains a set of test cases to verify that the scheme is working correctly.
These tests just:
- generate datasets for sender and receiver
- run the whole scheme
- output on a .csv file test result (and a simple time performance metrics), and print on terminal the result of the test

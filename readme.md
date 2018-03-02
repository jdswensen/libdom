# libdom

A simple DOM for HTML

[![Build Status](https://travis-ci.org/jdswensen/libdom.svg?branch=master)](https://travis-ci.org/jdswensen/libdom) [![codecov](https://codecov.io/gh/jdswensen/libdom/branch/master/graph/badge.svg)](https://codecov.io/gh/jdswensen/libdom)

## Compile and install

### Simple
Run `make` from the root of the project directory. To install, run `make install`. The demo program can then be run with `html2stream -i <INPUT_FILE>`

### Advanced
#### Requirements
- cmake
- lcov
- python

##### Compile and Test
All of these steps can be performed by using scripts in the tools directory. Run them from the root of the project directory with:
```
./tools/build.sh
./tools/run-tests.sh -c -d ./build-coverage -l ../tools/lcov-to-cobertura-xml/lcov_cobertura/lcov_cobertura.py
```

The run-test script performs the extra step of creating code coverage data.

##### Build
To compile with cmake run:
```
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release
make
```

##### Tests
The cmake tests are built with Google Test. They can be run individually or all at once by using the `ctest` command. All test binaries are prefixed with `test-`.

## Limitations
- Currently only handles valid, well formed HTML

## Improvements
- Use regex to sort through HTML tags
- Use a Document Type Definition (DTD) to validate input

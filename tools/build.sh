#!/bin/bash

set -e

timestamp=$(date +"%Y%m%dT%H%M%SZ")

new_build=build-release-${timestamp}
mkdir ${new_build}
cd ${new_build}
cmake .. -DCMAKE_BUILD_TYPE=Release
make
cd ..
ln -sf build-release-${timestamp} build

new_build=build-coverage-${timestamp}
mkdir ${new_build}
cd ${new_build}
cmake .. -DCMAKE_BUILD_TYPE=Debug -DCOVERAGE=ON
make
cd ..
ln -sf build-coverage-${timestamp} build-coverage

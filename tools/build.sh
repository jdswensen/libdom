#!/bin/bash

set -e

new_build=build-$(date +"%Y%m%dT%H%M%SZ")
mkdir ${new_build}
cd ${new_build}
cmake .. -DCMAKE_BUILD_TYPE=Release
make
cd ..

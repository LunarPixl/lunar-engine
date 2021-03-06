#!/bin/bash

set -euo pipefail

mkdir -p build && cd build

# Configure
cmake -DCODE_COVERAGE=ON -DCMAKE_BUILD_TYPE=Debug ..
# Build (for Make on Unix equivalent to `make -j $(nproc)`)
cmake --build . --config Debug -- -j $(nproc)

Xvfb :99 &
export DISPLAY=:99

# Test
ctest -j $(nproc) --output-on-failure -VV


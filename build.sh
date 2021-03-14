#!/bin/bash

set -euo pipefail

mkdir -p build && cd build

# Configure
cmake -DCODE_COVERAGE=ON -DCMAKE_BUILD_TYPE=Debug ..
# Build (for Make on Unix equivalent to `make -j $(nproc)`)
cmake --build . --config Debug -- -j $(nproc)

chmod u+x tools/img_compare.py
chmod u+x tools/img_diff.py
chmod u+x tools/img_crop.py
python3 -mvenv venv
./venv/Scripts/pip install -r requirements.txt
# Test
xvfb-run ctest -j $(nproc) --output-on-failure -VV


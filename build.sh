#!/bin/bash

set -euo pipefail

mkdir -p build && cd build

# Absolute path to this script, e.g. /home/user/bin/foo.sh
SCRIPT=$(readlink -f "$0")
# Absolute path this script is in, thus /home/user/bin
SCRIPTPATH=$(dirname "$SCRIPT")

# Configure
cmake -DCODE_COVERAGE=ON -DCMAKE_BUILD_TYPE=Debug ..
# Build (for Make on Unix equivalent to `make -j $(nproc)`)
cmake --build . --config Debug -- -j $(nproc)

chmod u+x tools/img_compare.py
chmod u+x tools/img_diff.py
chmod u+x tools/img_crop.py
python3 -mvenv venv
"${SCRIPTPATH}/venv/Scripts/pip" install -r requirements.txt
# Test
xvfb-run ctest -j $(nproc) --output-on-failure -VV


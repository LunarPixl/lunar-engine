#!/bin/bash

set -euo pipefail

mkdir -p build && cd build

cp -R ../tools .

# Absolute path to this script, e.g. /home/user/bin/foo.sh
SCRIPT=$(readlink -f "$0")
# Absolute path this script is in, thus /home/user/bin
SCRIPTPATH=$(dirname "$SCRIPT")

# Configure
cmake -DCODE_COVERAGE=ON -DCMAKE_BUILD_TYPE=Debug ..
# Build (for Make on Unix equivalent to `make -j $(nproc)`)
cmake --build . --config Debug -- -j $(nproc)

chmod u+x "${SCRIPTPATH}/tools/img_compare.py"
chmod u+x "${SCRIPTPATH}/tools/img_diff.py"
chmod u+x "${SCRIPTPATH}/tools/img_crop.py"

python3 -mvenv "${SCRIPTPATH}/venv"

#getting real tired of this
ls -l "${SCRIPTPATH}"
ls -l "${SCRIPTPATH}/venv"
ls -l "${SCRIPTPATH}/venv/Scripts"

"${SCRIPTPATH}/venv/Scripts/pip3" install -r requirements.txt
# Test
xvfb-run ctest -j $(nproc) --output-on-failure -VV


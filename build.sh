#!/bin/bash

set -euo pipefail

mkdir -p build && cd build

WORK_BUILD_DIR=.

if [ -z ${TRAVIS_BUILD_DIR+x}]; then
    WORK_BUILD_DIR=${TRAVIS_BUILD_DIR};
fi

if [ -z ${GITHUB_WORKSPACE+x}]; then
   WORK_BUILD_DIR=${GITHUB_WORKSPACE};
fi


# Configure
cmake -DCODE_COVERAGE=ON -DCMAKE_BUILD_TYPE=Debug ..
# Build (for Make on Unix equivalent to `make -j $(nproc)`)
cmake --build . --config Debug -- -j $(nproc)

chmod u+x tools/img_compare.py
chmod u+x tools/img_diff.py
chmod u+x tools/img_crop.py
python3 -mvenv venv
"${WORK_BUILD_DIR}/venv/Scripts/pip" install -r requirements.txt
# Test
xvfb-run ctest -j $(nproc) --output-on-failure -VV


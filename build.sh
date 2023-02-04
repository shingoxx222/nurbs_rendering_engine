#!/bin/bash

BUILD_DIR=tmp/build

cmake -B ${BUILD_DIR} -GNinja
cmake --build ${BUILD_DIR} --target all
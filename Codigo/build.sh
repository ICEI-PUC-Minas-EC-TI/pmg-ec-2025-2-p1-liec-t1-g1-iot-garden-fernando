#!/bin/bash
rm -rf server/build
cmake -B server/build -S server && cmake --build server/build

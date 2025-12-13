#!/bin/bash

# Compile Program.
cd ./build
cmake --build . --config Debug

# 3. Setup 
EXEC="alacritty -e bash -ic"

# 4. Execute compressor
 $EXEC "./compressor/compressor; exec bash" &

# 5. Execute sensor
 $EXEC "./sensor/sensor; exec bash" &

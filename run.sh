#!/bin/bash

# Compile Program.
cd ./build
cmake --build . --config Debug

# 3. Setup 
EXEC="alacritty -e bash -ic"

# 4. Execute server
$EXEC "./server; exec bash" &

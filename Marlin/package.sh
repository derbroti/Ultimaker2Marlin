#!/usr/bin/env bash

# This script is to package the Marlin package for Arduino
# This script should run under Linux and Mac OS X, as well as Windows with Cygwin.

#############################
# CONFIGURATION
#############################

##Which version name are we appending to the final archive
export BUILD_NAME=16.03.1

#############################
# Actual build script
#############################

MAKE=make


#############################
# Build the required firmwares
#############################

ARDUINO_PATH=/Users/Mirko/Downloads/Arduino-1.6.5/build/macosx/Arduino.app/Contents/Java
ARDUINO_VERSION=165


#Build the Ultimaker2 firmwares.

$MAKE -j 3 HARDWARE_MOTHERBOARD=72 ARDUINO_INSTALL_DIR=${ARDUINO_PATH} ARDUINO_VERSION=${ARDUINO_VERSION} BUILD_DIR=_Ultimaker2 clean
sleep 2
mkdir _Ultimaker2
$MAKE -j 3 HARDWARE_MOTHERBOARD=72 ARDUINO_INSTALL_DIR=${ARDUINO_PATH} ARDUINO_VERSION=${ARDUINO_VERSION} BUILD_DIR=_Ultimaker2 DEFINES="'STRING_CONFIG_H_AUTHOR=\"derbroti_${BUILD_NAME}+\"' TEMP_SENSOR_1=0 EXTRUDERS=1 FILAMENT_SENSOR_PIN=30 BABYSTEPPING HEATER_0_MAXTEMP=315 HEATER_1_MAXTEMP=315 HEATER_2_MAXTEMP=315 INVERT_E0_DIR=true INVERT_E1_DIR=true INVERT_E2_DIR=true"

mkdir -p build
mv _Ultimaker2/Marlin.hex build/derbroti-MarlinUltimaker2-${BUILD_NAME}.hex

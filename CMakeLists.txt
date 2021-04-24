# What CMake to start at
cmake_minimum_required(VERSION 3.12)

# Include the subsidiary .cmake file to get the SDK
include(pico_sdk_import.cmake)

# Initalise the SDK
pico_sdk_init()
 
# Set the name and version of the project
project(MidiOutTest VERSION 1.0.0)
 
# Link the Project to a source file (step 4.6)
add_executable(MidiOutTest main.c)
 
# Link the Project to an extra library (pico_stdlib)
target_link_libraries(MidiOutTest 
    pico_stdlib
)
 
target_compile_definitions(MidiOutTest PRIVATE
    PICO_DEFAULT_UART_TX_PIN=0
    PICO_DEFAULT_UART_RX_PIN=1
 )


# Create map/bin/hex file etc.)
pico_add_extra_outputs(MidiOutTest)
# WardenSTL - Simple C++ STL implementation for freestanding environment
This library is specially created for occasions where there is no standard library and where resources are limited. It implements the basic functions from the standard library and some that are useful but not present 
in it. It aims to support C++98/03 and bring some functions from newer versions, and implement features as many features from newer versions as possible. 

## Main Features
 - No dynamic memory allocation
 - No RTTI
 - Fixed size containers
 - CRC calculations
 - Checksum & hash functions
 - Error handling
 - Templates for buffers, streams
 - Very small usage of virtual functions. They are used only if they are really needed
 - Doxygen generated documentation
 - Simple threading
 - Header-only implementation

## Installation
There is nothing special. Just copy or clone the GitHub project into appropriate directory. All headers are located in `include` folder.

## Generating documentation
You should have [SCons](https://scons.org/) installed. Issue the `scons` command to generate documentation. To clean everything, just execute `scons -c` command.

## Acknowledgments
This library was inspired by the [Embedded Template Library (ETL)](https://github.com/ETLCPP/etl), and some parts of the code were adapted from it.

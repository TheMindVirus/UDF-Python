# UDF-Python
CPython running as an interpreter for a UMDFv2 Windows Driver!
Continuation of KMDF-X: Windows Diagnostic Drivers for Raspberry Pi 4 in Low Peripheral Mode

## Please use with caution, this repository is indended for educational purposes and testing. ##

This project may be used as a template to aid the prototyping of production drivers for ARM64 platforms similar to the Raspberry Pi 4.

## Requirements ##

Visual Studio 2019 Community Edition with ARM64 WDK Installed (and C++ ARM64 Build Tools with Spectre Mitigated Libs)

Test Signing On with Windows 10 ARM Insider Build 21277 in Test Mode

Inf2Cat On with /uselocaltime

Wpp Tracing Off in favour of Text File Debug

Python C++ Interface is left deliberately incomplete due to methods being made accessible by the Python Interpreter.
Additional interfaces can be added to e.g. perform Device Memory Access with READ_REGISTER_ULONG() to aid driver prototyping.

## Installation ##

The CopyFiles folder contains a miniature distribution of cpython and devcon arm64, along with a batch file installer.

To run the installer, copy the .../ARM64/Release/UDF/ directory onto the Raspberry Pi 4 (or access from a network drive) and run the batch file.

It will install both Python-arm64 and the UMDF driver to .../System32/drivers/UMDF/Python/ accessible from the console.

To uninstall the driver, use Device Manager to right-click and uninstall the device, then optionally delete the Python directory.

Multiple instances of the driver can be installed simultaneously and by default they will log to the same file in %WinDir%/Temp.

### You may be prompted with a security warning if driver signature enforcement is enabled. ###
### Please only follow these instructions if you are testing Windows and you know what you are doing. ###

# PMTTYDR AngelScript Registry Dumper
A homebrew application to dump Paper Mario: The Thousand Year Door (2024)'s AngelScript registry from memory to JSON.

## Usage
Download the .nro file from the releases section, drop it into the "switch" directory in the root of your SD card.

After using the program, it will generate a "ttydr_output.json" file in your SD card root directory. This is to be used with the "ttydr_output2ghidra.py" script to label all the found functions and data.

## Building
Follow the SwitchBrew "[Setting up Development Environment](https://switchbrew.org/wiki/Setting_up_Development_Environment)" guide to install Switch homebrew development libraries and tools. Ensure you have the "DEVKITPRO" environment variable set to your devKitPro installation directory. You can then run the "make" command following the installation.


## Project Structure
* Source - Source files.
* Include - Include files.
* Scripts - Scripts to help streamline development and interaction with output.
* ThirdParty - Libraries from other projects.

## Credits
This repository makes use of the following open source projects:
* [Atmosphere-NX/Atmosphere-libs](https://github.com/Atmosphere-NX/Atmosphere-libs) - Headers for interacting with the dmntcht interface.
* [nlohmann/json](https://github.com/nlohmann/json) - C++ single-include JSON header.

## Notes
* Only supports game version 1.0.1.
* Ghidra script only tested with version 11.3.2.
* Building only tested on Windows 10.
* Currently only dumps global functions, object functions, and global properties.

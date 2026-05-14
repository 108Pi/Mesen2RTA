# Mesen2RTA

Speedrunning friendly version of Mesen2

# Mesen

Mesen is a multi-system emulator (NES, SNES, Game Boy, Game Boy Advance, PC Engine, SMS/Game Gear, WonderSwan) for Windows, Linux and macOS.  

## Development Builds

[![Mesen](https://github.com/SourMesen/Mesen2/actions/workflows/build.yml/badge.svg)](https://github.com/SourMesen/Mesen2/actions/workflows/build.yml)

#### <ins>Native builds</ins> (recommended) ####

These builds don't require .NET to be installed.  

* [Windows 10 / 11](https://nightly.link/SourMesen/Mesen2/workflows/build/master/Mesen%20%28Windows%20-%20net8.0%20-%20AoT%29.zip)  
* [Linux x64](https://nightly.link/SourMesen/Mesen2/workflows/build/master/Mesen%20%28Linux%20-%20ubuntu-22.04%20-%20clang_aot%29.zip)  (requires **SDL2**)
* [macOS - Intel](https://nightly.link/SourMesen/Mesen2/workflows/build/master/Mesen%20%28macOS%20-%20macos-13%20-%20clang_aot%29.zip)  (requires **SDL2**)
* [macOS - Apple Silicon](https://nightly.link/SourMesen/Mesen2/workflows/build/master/Mesen%20%28macOS%20-%20macos-14%20-%20clang_aot%29.zip)  (requires **SDL2**)

#### <ins>.NET builds</ins> ####

These builds require **.NET 8** to be installed (except the Windows 7 build which requires .NET 6).  
For Linux and macOS, **SDL2** must also be installed.

* [Windows 7 / 8 (.NET 6)](https://nightly.link/SourMesen/Mesen2/workflows/build/master/Mesen%20%28Windows%20-%20net6.0%29.zip)  
* [Linux x64 - AppImage](https://nightly.link/SourMesen/Mesen2/workflows/build/master/Mesen%20(Linux%20x64%20-%20AppImage).zip)  
* [Linux ARM64](https://nightly.link/SourMesen/Mesen2/workflows/build/master/Mesen%20%28Linux%20-%20ubuntu-22.04-arm%20-%20clang%29.zip)  
* [Linux ARM64 - AppImage](https://nightly.link/SourMesen/Mesen2/workflows/build/master/Mesen%20(Linux%20ARM64%20-%20AppImage).zip)


#### <ins>Notes</ins> ####

Other builds are also available in the [Actions](https://github.com/SourMesen/Mesen2/actions) tab.

**SteamOS**: See [SteamOS.md](SteamOS.md)

## Compiling

See [COMPILING.md](COMPILING.md)

## Timer Info
The built in timer can be customized through timer.txt  
This is done with sets of conditions, and if none of the conditions are false, it sets the timer to the indicated state  
The included timer.txt has example conditions for Super Mario Bros.   
Note the emulator must be closed and reopened for changes in timer.txt to take effect

### Condition Types
start - sets the timer to start running  
stop - stops the timer from running. It may be started again if a start condition is met  
reset - resets the timer to the start frame (0 by default) and stops it  
end - stops the timer from running. It cannot be started again without resetting  
vpause - visually pauses the timer for some number of frames (60 by default). After, it will continue to run as if it hadn't been stopped  

### Conditions
Each condition is an address, a comparison operator, and a value  
Possible comparison operators are = or ==, <, >, <=, >=, and !=  

### Syntax 
Whitespace isn't needed and is ignored  
Numbers can be prefaced with 0x to signify hex  
//This is a comment

The syntax for lists of conditions is
```
<type> { <condition>* }
<condition> => <number> <operator> <number>
```
### Timer Settings
```delay <number>``` can be used to set a negative starting point  
```advance <number>``` can be used for a positive starting point  
```pausetime <number>``` changes the number of frames vpause stops the timer for

### Variables 
Numbers can be assiged to variables with ```let <var name> = <number>```   
Variables can be used in place of numbers in conditions or settings  
Variables must be alphanumberic and cannot start with a digit

## License

Mesen is available under the GPL V3 license.  Full text here: <http://www.gnu.org/licenses/gpl-3.0.en.html>

Copyright (C) 2014-2025 Sour

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

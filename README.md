# Mesen Community Edition
# Mesen2RTA

Speedrunning friendly version of Mesen2

# Mesen

Mesen is a multi-system emulator for Windows, Linux, and macOS. It supports NES, SNES, Game Boy (GB/SGB/GBC), Game Boy Advance, PC Engine, SMS/Game Gear, and WonderSwan (WS/WSC).

This is a community-managed fork, created to maintain and expand this emulator into the future.

## Releases

The latest stable version is available from the [releases page on GitHub](https://github.com/nesdev-org/MesenCE/releases).

## Development Builds

[![Mesen](https://github.com/nesdev-org/MesenCE/actions/workflows/build.yml/badge.svg)](https://github.com/nesdev-org/MesenCE/actions/workflows/build.yml?query=branch%3Amaster)

* [Windows](https://nightly.link/nesdev-org/MesenCE/workflows/build/master/Mesen%20%28Windows%20-%20net10.0%20-%20AoT%29.zip)
  * Windows 7 or higher is required. Windows 7 users must use SP1 and have all updates installed.
* [Linux x64](https://nightly.link/nesdev-org/MesenCE/workflows/build/master/Mesen%20%28Linux%20-%20ubuntu-22.04%20-%20clang_aot%29.zip)  (requires **SDL2**)  
* [Linux ARM64](https://nightly.link/nesdev-org/MesenCE/workflows/build/master/Mesen%20%28Linux%20-%20ubuntu-22.04-arm%20-%20clang_aot%29.zip)  (requires **SDL2**)  
* [macOS - Intel](https://nightly.link/nesdev-org/MesenCE/workflows/build/master/Mesen%20%28macOS%20-%20macos-15-intel%20-%20clang_aot%29.zip)  (requires **SDL2**)  
* [macOS - Apple Silicon](https://nightly.link/nesdev-org/MesenCE/workflows/build/master/Mesen%20%28macOS%20-%20macos-15%20-%20clang_aot%29.zip)  (requires **SDL2**)  

#### <ins>Notes</ins> ####

* Other builds are also available in the [Actions](https://github.com/nesdev-org/MesenCE/actions/workflows/build.yml?query=branch%3Amaster) tab.
* **macOS**: Builds are self-signed and will require approval via Gatekeeper before they are able to be run.  
* **SteamOS**: See [SteamOS.md](SteamOS.md)  

## Compiling

See [COMPILING.md](COMPILING.md)

## Contributing

See [CONTRIBUTING.md](CONTRIBUTING.md)
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

Copyright (C) 2014-2026 Sour, 2026 contributors

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

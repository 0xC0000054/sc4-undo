# sc4-undo

## This was an April fools day joke.
In response to the _Control + Z_ shortcut it show an in-game message box with "Happy April Fools Day.".

It now serves as a demo of how a DLL can add its own keyboard shortcuts to the game.   
The plugin can be downloaded from the Releases tab: https://github.com/0xC0000054/sc4-undo/releases

## System Requirements

* Windows 10 or later

## Installation

1. Close SimCity 4.
2. Copy `SC4Undo.dll` and `Undo.dat` into the Plugins folder in the SimCity 4 installation directory.
3. Start SimCity 4.

## Usage

Press _Control + Z_ while in the city view.

# License

This project is licensed under the terms of the MIT License.    
See [LICENSE.txt](LICENSE.txt) for more information.

## 3rd party code

[gzcom-dll](https://github.com/nsgomez/gzcom-dll/tree/master) Located in the vendor folder, MIT License.    
[SC4Fix](https://github.com/nsgomez/sc4fix) - MIT License.    

# Source Code

## Prerequisites

* Visual Studio 2022

## Building the plugin

* Open the solution in the `src` folder
* Update the post build events to copy the build output to you SimCity 4 application plugins folder.
* Build the solution

## Debugging the plugin

Visual Studio can be configured to launch SimCity 4 on the Debugging page of the project properties.
I configured the debugger to launch the game in a window with the following command line:    
`-intro:off -CPUcount:1 -w -CustomResolution:enabled -r1920x1080x32`

You may need to adjust the resolution for your screen.

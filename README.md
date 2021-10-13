# PapyrusExtenderSSE

SKSE64 plugin that expands Papyrus script functionality, with 276 functions, 29 events, 4 script objects for Skyrim Special Edition. 

Documentation for each function is listed within the Papyrus source files (.psc extension).

## Requirements
* [CMake](https://cmake.org/)
	* Add this to your `PATH`
* [PowerShell](https://github.com/PowerShell/PowerShell/releases/latest)
* [Vcpkg](https://github.com/microsoft/vcpkg)
	* Add the environment variable `VCPKG_ROOT` with the value as the path to the folder containing vcpkg
* [Visual Studio Community 2019](https://visualstudio.microsoft.com/)
	* Desktop development with C++
* [CommonLibSSE](https://github.com/powerof3/CommonLibSSE/tree/dev)
	* You need to build from the powerof3/dev branch
	* Add this as as an environment variable `CommonLibSSEPath`

## Register Visual Studio as a Generator
* Open `x64 Native Tools Command Prompt`
* Run `cmake`
* Close the cmd window

## Building
```
git clone https://github.com/powerof3/PapyrusExtenderSSE.git
cd PapyrusExtenderSSE
cmake -B build -S .
```
Open build/po3_PapyrusExtender.sln in Visual Studio to build dll.

## License
[MIT](LICENSE)

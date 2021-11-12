# PapyrusExtenderSSE

SKSE64/VR plugin that expands Papyrus script functionality with 442 functions, 82 events, and 4 script objects for Skyrim Special Edition.

Documentation for each function is listed within the Papyrus source files (.psc extension).

* **[Download on Nexus Mods!](https://www.nexusmods.com/skyrimspecialedition/mods/22854)**
* [SKSEVR version](https://www.nexusmods.com/skyrimspecialedition/mods/58296)


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
* [CommonLibVR](https://github.com/alandtse/CommonLibVR/tree/vr)
	* Add this as as an environment variable `CommonLibVRPath`

## User Requirements
* [Address Library for SKSE](https://www.nexusmods.com/skyrimspecialedition/mods/32444)
	* Needed for SSE
* [VR Address Library for SKSEVR](https://www.nexusmods.com/skyrimspecialedition/mods/58101)
	* Needed for VR
## Register Visual Studio as a Generator
* Open `x64 Native Tools Command Prompt`
* Run `cmake`
* Close the cmd window

### SSE
```
cmake -B build -S .
```
Open build/po3_PapyrusExtender.sln in Visual Studio to build dll.

### VR
```
cmake -B build2 -S . -DBUILD_SKYRIMVR=On
```
Open build2/po3_PapyrusExtender.sln in Visual Studio to build dll.


## License
[MIT](LICENSE)

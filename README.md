# PapyrusExtenderSSE

SKSE64/VR plugin that expands Papyrus script functionality with 442 functions, 82 events, and 4 script objects for Skyrim Special Edition.

Documentation for each function is listed in the [wiki](https://github.com/powerof3/PapyrusExtenderSSE/wiki).

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
	* Needed for SSE/AE
* [VR Address Library for SKSEVR](https://www.nexusmods.com/skyrimspecialedition/mods/58101)
	* Needed for VR
## Register Visual Studio as a Generator
* Open `x64 Native Tools Command Prompt`
* Run `cmake`
* Close the cmd window

## Building
```
git clone https://github.com/powerof3/PapyrusExtenderSSE.git
cd PapyrusExtenderSSE
# pull commonlib, skip if you've defined it in the path
git submodule init
# to update submodules to checked-out build (warning, pulling newer verisons may result in build problems)
git submodule update
```

### SSE
```
cmake --preset vs2022-windows-vcpkg # for vs2019 use vs2019-windows-vcpkg
cmake --build build --config Release
```
### AE
```
cmake --preset vs2022-windows-vcpkg # for vs2019 use vs2019-windows-vcpkg
cmake --build buildae --config Release
```
### VR
```
cmake --preset vs2022-windows-vcpkg-vr # for vs2019 use vs2019-windows-vcpkg-vr
cmake --build buildvr --config Release
```

## License
[MIT](LICENSE)

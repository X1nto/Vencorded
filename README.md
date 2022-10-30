# Vencorded
The best Vencord installer on the market (there's only 3 but whatever).

Linux and macOS support, as well as git integration are coming soon™.

# Building
First of all, you will need to install [cmake](https://cmake.org/) and [vcpkg](https://vcpkg.io/)

## Windows
Run the following commands:
```powershell
mkdir build
cd build
cmake .. -G "Visual Studio 17 2022" -DCMAKE_TOOLCHAIN_FILE=[path to vcpkg]/scripts/buildsystems/vcpkg.cmake
cmake --build .
```
The output will be in the /bin directory of /build
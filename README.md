# Vencorded
The best Vencord installer on the market (there's only 3 but whatever).

Linux and macOS support, as well as git integration are coming soon™.

# Building
First of all, you will need to install [cmake](https://cmake.org/) and [conan](https://conan.io/)

## Windows
Run the following commands:
```powershell
mkdir build
cd build
conan install ..\conanfile_win32.txt --build=pdcurses
cmake .. -G "Visual Studio 17 2022" #You can specify any generator here, for the sake of this example, we use VS2022 build tools
cmake --build .
```
The output will be in the /bin directory of /build
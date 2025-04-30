@echo off

pushd ..
Walnut\vendor\bin\premake\Windows\premake5.exe --file=build.lua vs2022
Walnut\vendor\bin\premake\Windows\premake5.exe --file=build-headless.lua vs2022
popd
pause
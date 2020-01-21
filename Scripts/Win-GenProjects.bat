@echo off
pushd %~dp0\..\
call vendor\bin\premake\premake5.exe --os=windows vs2019
popd
PAUSE
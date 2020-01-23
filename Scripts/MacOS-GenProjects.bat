@echo off
pushd %~dp0\..\
call vendor\bin\Windows\premake\premake5.exe --os=macosx gmake2
popd
PAUSE
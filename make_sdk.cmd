@echo off
echo Making SDK, a moment please...

rd /s /q sdk
mkdir sdk
xcopy framework sdk /E /Y

del /f /s /q sdk\gef.sln

del /f /s /q sdk\gef_common_message\*.cpp
del /f /s /q sdk\gef_common_message\*.vcproj

del /f /q sdk\gef_dot_net\*.*

del /f /s /q sdk\gef_service_base\*.cpp
del /f /s /q sdk\gef_service_base\*.vcproj

del /f /q sdk\gef_shell\*.*
rd /s /q sdk\gef_shell\Properties

rd /s /q sdk\gef_standard_plugin\gef_plugin_system
rd /s /q sdk\gef_standard_plugin\gef_plugin_file
rd /s /q sdk\gef_standard_plugin\gef_plugin_edit
rd /s /q sdk\gef_standard_plugin\gef_plugin_script
rd /s /q sdk\gef_standard_plugin\gef_plugin_help

echo SDK made!
echo. & pause

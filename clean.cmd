@echo off
echo Cleaning, a moment please...

attrib *.suo -s -h -r

del /f /s /q *.exp
del /f /s /q *.ncb
del /f /s /q *.suo
del /f /s /q *.user

rd /s /q temp
rd /s /q framework\gef_shell\obj
rd /s /q framework\gef_test_case\gef_plugin_test_case\obj
rd /s /q framework\gef_standard_plugin\gef_plugin_system\obj
rd /s /q framework\gef_standard_plugin\gef_plugin_file\obj
rd /s /q framework\gef_standard_plugin\gef_plugin_edit\obj
rd /s /q framework\gef_standard_plugin\gef_plugin_script\obj
rd /s /q framework\gef_standard_plugin\gef_plugin_help\obj

del /f /s /q output\*.dep
del /f /s /q output\*.idb
del /f /s /q output\*.ilk
del /f /s /q output\*.manifest
del /f /s /q output\*.obj
del /f /s /q output\*.pdb
del /f /s /q output\*.res
del /f /s /q output\*.vshost.exe

echo Cleaning done!
echo. & pause

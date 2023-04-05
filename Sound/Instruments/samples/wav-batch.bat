@echo off

@cd %~dp0
@cd convert
@dir *.wav /b > wav.txt
@cd %~dp0
@echo Running wav2agb...
for /f "tokens=*" %%m in (%~dp0/convert/wav.txt) do wav2agb -s wav --rate 13379 --key 84 "%~dp0/convert/%%m"
@cd convert
@del "wav.txt"
@dir *.s /b > s.txt
@echo Creating .dmp files...
for /f "tokens=*" %%m in (%~dp0/convert/s.txt) do echo: | (AssembleARM.bat "%~dp0/convert/%%m")
@del "s.txt"
pause

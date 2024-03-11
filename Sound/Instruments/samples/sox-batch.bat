@echo off

@cd %~dp0
@cd convert
@dir *.wav /b > wav.txt
@cd %~dp0/sox-14.4.2
@echo Running sox...
for /f "tokens=*" %%m in (%~dp0/convert/wav.txt) do sox %~dp0/convert/%%m -r 13379 -c 1 %%m.sb
@cd %~dp0/convert
@del "wav.txt"
pause
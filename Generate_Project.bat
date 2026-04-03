@ECHO OFF


ECHO Copying Dependencies...

MKDIR Bin\
MKDIR Bin\Debug\
MKDIR Bin\Release\

XCOPY Dependencies\* Bin\ /dys

ECHO Finished Copying Dependencies!
ECHO:


ECHO Generating Project Files...

CALL "Premake/premake5.exe" --file=Premake/workspace.lua vs2022

ECHO Finished Generating Project Files!
ECHO:


PAUSE

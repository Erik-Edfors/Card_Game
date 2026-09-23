@ECHO OFF

ECHO Building Assets...
ECHO:

ECHO Building Fonts...

Tools\AssetBuilder.exe "RawAssets/Font/" "Assets/Font/" font "--regex=^.*\.ttf$" "--extension=.bf" --emsize=24.0 --sdlength=4.0 /rp

ECHO Finished building Fonts!
ECHO:

ECHO Finished building Assets!

PAUSE
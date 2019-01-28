cd..
cd..

xcopy .\Editor\Bin\*.* .\Client_3D\Bin\ /s /y /d /exclude:exclude.txt
xcopy .\Editor\Bin\*.* .\Engine\Bin\ /s /y /d /exclude:exclude.txt
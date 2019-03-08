cd..

xcopy .\Mathlib\Include\*.h .\DX11_Engine\Include\ /s /y /d
xcopy .\Mathlib\Include\*.h .\Engine\Include\ /s /y /d

xcopy .\Mathlib\Bin\*.* .\Client_3D\Bin\ /s /y /d /exclude:exclude.txt
xcopy .\Mathlib\Bin\*.* .\Engine\Bin\ /s /y /d /exclude:exclude.txt
xcopy .\Mathlib\Bin\*.* .\Editor\Bin\ /s /y /d /exclude:exclude.txt
cd..
cd..
	
xcopy .\Engine\Include\*.h .\DX11_Engine\Include\ /s /y /d
xcopy .\Engine\Bin\*.* .\DX11_Engine\Bin\ /s /y /d /exclude:exclude.txt
xcopy .\Engine\Bin\*.* .\Client\Bin\ /s /y /d /exclude:exclude.txt
xcopy .\Engine\Bin\*.* .\Editor\Bin\ /s /y /d /exclude:exclude.txt
xcopy .\Client\Bin\*.* .\Editor\Bin\ /s /y /d /exclude:exclude.txt
xcopy .\Client_3D\Bin\*.* .\Editor\Bin\ /s /y /d /exclude:exclude.txt
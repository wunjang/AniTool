rmdir /s /q "ipch"
rmdir /s /q "Debug"
rmdir /s /q "Release"

rmdir /s /q .\Client\Defualt\Debug
rmdir /s /q .\Client\Defualt\ipch
rmdir /s /q .\Client\Defualt\Release
rmdir /s /q .\Client\Defualt\x64

rmdir /s /q .\MFC\Defualt\Debug
rmdir /s /q .\MFC\Defualt\ipch
rmdir /s /q .\MFC\Defualt\Release
rmdir /s /q .\MFC\Defualt\x64

rmdir /s /q .\Engine\Resources\Defualt\Debug
rmdir /s /q .\Engine\System\Defualt\Debug
rmdir /s /q .\Engine\Utility\Defualt\Debug

del /f /q .\Client\Bin\Client.exe
del /f /q .\Client\Bin\Client.ilk
del /f /q .\Client\Bin\Client.pdb

del /f /q .\MFC\Bin\MFC.exe
del /f /q .\MFC\Bin\MFC.ilk
del /f /q .\MFC\Bin\MFC.pdb

del /f /q Frame103.VC.db

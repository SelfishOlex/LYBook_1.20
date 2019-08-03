.\lmbr_waf.bat build_win_x64_vs2017_profile -p all
.\lmbr_waf.bat build_win_x64_vs2017_release -p game_and_engine

if (Test-Path -Path .\myproject_pc_paks\) {
    Remove-Item .\myproject_pc_paks\ -Recurse -Force
}

.\BuildMyProject_Paks_PC.bat
Copy-Item .\Bin64vc141.Release\ `
    -Destination .\myproject_pc_paks\ -Recurse -Force -Verbose

.\lmbr_pak_shaders.bat MyProject D3D11 pc
Copy-Item .\Build\pc\MyProject\*.pak `
    -Destination .\myproject_pc_paks\myproject\ `
    -Force -Verbose

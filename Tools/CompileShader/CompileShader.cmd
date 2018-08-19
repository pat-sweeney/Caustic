REM %1 - type of shader (PS=pixel shader VS=vertex shader)
REM %2 - name of shader (e.g. Default)
REM Example: $(CausticRoot)\Tools\CompileShader\CompileShader.cmd PS Defaults
if ["%1"]==["PS"] GOTO PS
GOTO VS
:VS
echo Processing VertexShader
set ShaderFilename=.\%2.vs
set ShaderTblFilename=.\%2VS.tbl
set ShaderTblVar=g_%2VS_ParamTable
set ShaderByteTbl=g_%2VS
set ShaderByteHdr=%2VS.h
echo %CausticRoot%/x64/Release/ParseShader.exe -i %ShaderFilename% -o .\%ShaderTblFilename% -v %ShaderTblVar%
%CausticRoot%/x64/Release/ParseShader.exe -i %ShaderFilename% -o .\%ShaderTblFilename% -v %ShaderTblVar%
echo fxc.exe /Zi /E"VS" /Od /Vn"%ShaderByteTbl%" /Fo"%CausticRoot%\Debug\Default.cso" /Tvs_5_0" /Fh"%ShaderByteHdr%" /nologo %ShaderFilename%
fxc.exe /Zi /E"VS" /Od /Vn"%ShaderByteTbl%" /Fo"%CausticRoot%\Debug\Default.cso" /T"vs_5_0" /Fh"%ShaderByteHdr%" /nologo %ShaderFilename%
goto DONE
:PS
echo Processing PixelShader
set ShaderFilename=.\%2.ps
set ShaderTblFilename=.\%2PS.tbl
set ShaderTblVar=g_%2PS_ParamTable
set ShaderByteTbl=g_%2PS
set ShaderByteHdr=%2PS.h
echo %CausticRoot%/x64/Release/ParseShader.exe -i %ShaderFilename% -o .\%ShaderTblFilename% -v %ShaderTblVar%
%CausticRoot%/x64/Release/ParseShader.exe -i %ShaderFilename% -o .\%ShaderTblFilename% -v %ShaderTblVar%
echo fxc.exe /Zi /E"PS" /Od /Vn"%ShaderByteTbl%" /Fo"%CausticRoot%\Debug\Default.cso" /T"ps_4_0_level_9_3" /Fh"%ShaderByteHdr%" /nologo %ShaderFilename%
fxc.exe /Zi /E"PS" /Od /Vn"%ShaderByteTbl%" /Fo"%CausticRoot%\Debug\Default.cso" /T"ps_4_0_level_9_3" /Fh"%ShaderByteHdr%" /nologo %ShaderFilename%
:DONE

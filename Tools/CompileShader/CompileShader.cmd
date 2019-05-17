REM %1 - Target (e.g. Debug or Release)
REM %2 - name of shader (e.g. Default)
REM Example: $(CausticRoot)\Tools\CompileShader\CompileShader.cmd Debug Default
echo Compiling VertexShader
echo fxc.exe /Zi /E"VS" /Od /Fo"%CausticRoot%\%1\%2_VS.cso" /Tvs_5_0" /nologo %2.vs
fxc.exe /Zi /E"VS" /Od /Fo"%CausticRoot%\%1\%2_VS.cso" /T"vs_5_0" /nologo %2.vs

echo Compiling PixelShader
echo fxc.exe /Zi /E"PS" /Od /Fo"%CausticRoot%\%1\%2_PS.cso" /T"ps_5_1" /nologo %2.ps
fxc.exe /Zi /E"PS" /Od /Fo"%CausticRoot%\%1\%2_PS.cso" /T"ps_5_1" /nologo %2.ps

echo Generating ShaderInfo
echo %CausticRoot%/x64/Release/ParseShader.exe -i %2 -o %CausticRoot%\%1\%2.shi
%CausticRoot%/x64/Release/ParseShader.exe -i %2 -o %CausticRoot%\%1\%2.shi

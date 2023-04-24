copy %CausticRoot%\DocumentOverrides\home.html %CausticRoot%\docs\other\home.html
copy %CausticRoot%\DocumentOverrides\LayerGraph.png %CausticRoot%\docs\other\LayerGraph.png
echo %CausticRoot%\%1\postprocdoc.exe %CausticRoot%\docs
%CausticRoot%\%1\postprocdoc.exe %CausticRoot%\docs
exit 0
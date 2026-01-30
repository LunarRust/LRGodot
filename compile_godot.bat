scons/scons.bat platform=windows target=editor module_mono_enabled=yes module_godot_tracy_enabled=true
REM # Build export templates
scons/scons.bat platform=windows target=template_debug module_mono_enabled=yes module_godot_tracy_enabled=true
scons/scons.bat platform=windows target=template_release module_mono_enabled=yes module_godot_tracy_enabled=true
REM # Generate glue sources
.\bin\godot.windows.editor.x86_64.mono.exe --headless --generate-mono-glue modules/mono/glue
REM # Build .NET assemblies
python .\modules\mono\build_scripts\build_assemblies.py --godot-output-dir .\bin --push-nupkgs-local C:\Users\je0976317\NuGet\MyLocalNugetSource
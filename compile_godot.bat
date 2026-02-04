scons/scons.bat platform=windows target=editor module_mono_enabled=yes module_godot_tracy_enabled=true debug_symbols=yes separate_debug_symbols=yes module_godot_tracy_enabled=true profiler=tracy profiler_path="C:\Users\je0976317\Godot\Tools\tracy"
REM # Build export templates
scons/scons.bat platform=windows target=template_debug module_mono_enabled=yes debug_symbols=yes separate_debug_symbols=yes module_godot_tracy_enabled=true profiler=tracy profiler_path="C:\Users\je0976317\Godot\Tools\tracy"
scons/scons.bat platform=windows target=template_release module_mono_enabled=yes
REM # Generate glue sources
.\bin\godot.windows.editor.x86_64.mono.exe --headless --generate-mono-glue modules/mono/glue
REM # Build .NET assemblies
python .\modules\mono\build_scripts\build_assemblies.py --godot-output-dir .\bin --push-nupkgs-local C:\Users\je0976317\NuGet\MyLocalNugetSource
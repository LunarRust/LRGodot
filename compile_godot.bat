call scons/scons.bat platform=windows target=editor d3d12=yes module_mono_enabled=yes production=yes dev_build=no extra_suffix=lunarust_production &&^
REM # Build export templates
call scons/scons.bat platform=windows target=template_debug d3d12=yes module_mono_enabled=yes debug_symbols=yes num_jobs=20 profiler=tracy profiler_path="%USERPROFILE%\godot\tracy" &&^
call scons/scons.bat platform=windows target=template_release d3d12=yes module_mono_enabled=yes num_jobs=12 &&^
REM # Generate glue sources
call .\bin\godot.windows.editor.x86_64.lunarust_production.mono.exe --headless --generate-mono-glue modules/mono/glue &&^
REM # Build .NET assemblies
call python .\modules\mono\build_scripts\build_assemblies.py --godot-output-dir .\bin --push-nupkgs-local %USERPROFILE%\NuGet\lunarust_nuget\godot_nuget_store --godot-platform=windows
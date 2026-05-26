scons platform=linuxbsd target=editor module_mono_enabled=yes module_godot_tracy_enabled=true production=yes dev_build=no module_godot_tracy_enabled=true
bin/godot.linuxbsd.editor.x86_64.mono --headless --generate-mono-glue modules/mono/glue
./modules/mono/build_scripts/build_assemblies.py --godot-output-dir=./bin
dotnet nuget add source /home/ashley/.nuget/godot_nuget_store/ --name LRGodotNugetSource

scons platform=linuxbsd target=editor module_mono_enabled=yes module_godot_tracy_enabled=true production=yes dev_build=no module_godot_tracy_enabled=true extra_suffix=lunarust_production
dotnet nuget add source $HOME/.lunarust_nuget/godot_nuget_store/ --name LRGodotNugetSource
bin/godot.linuxbsd.editor.x86_64.mono --headless --generate-mono-glue modules/mono/glue
./modules/mono/build_scripts/build_assemblies.py --godot-output-dir=./bin --push-nupkgs-local $HOME/.lunarust_nuget/godot_nuget_store/ --godot-platform=linuxbsd


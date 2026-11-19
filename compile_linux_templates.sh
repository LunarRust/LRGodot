export SCRIPT_AES256_ENCRYPTION_KEY="79798E9C6B0324ED84F3BEA18344116C8537054BA5386C20E1BF8D2D2ECE2AB5"

scons platform=linuxbsd target=template_debug module_mono_enabled=yes debug_symbols=yes num_jobs=12 production=yes dev_build=no profiler=tracy profiler_path=/run/media/octo/980Evo/tracy extra_suffix=lunarust_production
scons platform=linuxbsd target=template_release module_mono_enabled=yes production=yes dev_build=no extra_suffix=lunarust_production
dotnet nuget add source $HOME/.lunarust_nuget/godot_nuget_store/ --name LRGodotNugetSource
bin/godot.linuxbsd.editor.x86_64.lunarust_production.mono --headless --generate-mono-glue modules/mono/glue
./modules/mono/build_scripts/build_assemblies.py --godot-output-dir=./bin --push-nupkgs-local $HOME/.lunarust_nuget/godot_nuget_store/ --godot-platform=linuxbsd


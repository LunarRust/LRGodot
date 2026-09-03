cd /run/media/octo/980Evo/LRGodot/
scons platform=linuxbsd target=editor production=yes dev_build=no num_jobs=12 use_static_cpp=yes extra_suffix=lunarust_production
scons platform=linuxbsd target=template_release arch=x86_64 production=yes dev_build=no num_jobs=12 use_static_cpp=yes extra_suffix=lunarust_production
scons platform=linuxbsd target=template_debug arch=x86_64 production=yes dev_build=no num_jobs=12 use_static_cpp=yes extra_suffix=lunarust_production

scons platform=windows target=editor production=yes dev_build=no d3d12=yes num_jobs=12 use_static_cpp=yes extra_suffix=lunarust_production
scons platform=windows target=template_release arch=x86_64 production=yes dev_build=no d3d12=yes num_jobs=12 extra_suffix=lunarust_production
scons platform=windows target=template_debug arch=x86_64 production=yes dev_build=no d3d12=yes num_jobs=12 extra_suffix=lunarust_production

bin/godot.linuxbsd.editor.x86_64.mono --headless --generate-mono-glue modules/mono/glue
modules/mono/build_scripts/build_assemblies.py --godot-output-dir=bin/ --push-nupkgs-local /home/octo/NuGetSource/ --godot-platform=linuxbsd

# MeshIO
[CloudCompare](https://github.com/CloudCompare/CloudCompare) plugin for loading COLLADA, glTF, and IFC meshes.

This is a plugin for [CloudCompare](https://github.com/CloudCompare/CloudCompare) to add reading of COLLADA, glTF, and IFC meshes. It uses the [Open Asset Import Library](https://github.com/assimp/assimp) (Assimp).

## Building

- clone this repository in the `CloudCompare/plugins/3rdParty` directory
- make sure the submodule `extern/assmip` is up-to-date by `git pull`ing it
- re-run CloudCompare's cmake
- turn on `PLUGIN_MESH_IO` in your cmake options
- build CloudCompare

## Caveat

This plugin is using another library to read these file formats. There will be problems with some files (e.g. IFC is not 100% implemented). It also means that some mesh names might be lost when importing some formats.

Because this plugin is basically translating the Assimp data into something CloudCompare understands, it can be tricky to figure out where the problem lies.

If you find files that do not work as expected, please add an [issue here](https://github.com/asmaloney/MeshIO/issues) and attach the smallest example you have that isn't working. I will try to assess where the problem lies and either report it to the [Assimp](https://github.com/assimp/assimp) project or [CloudCompare](https://github.com/CloudCompare/CloudCompare) project as necessary.

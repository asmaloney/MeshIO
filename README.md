[![CloudCompare Plugin](https://img.shields.io/badge/plugin-CloudCompare-brightgreen.svg)](https://github.com/CloudCompare/CloudCompare)
[![License](https://img.shields.io/badge/License-BSD%203--Clause-blue.svg)](https://opensource.org/licenses/BSD-3-Clause)

# <img src="https://github.com/asmaloney/MeshIO/blob/master/images/icon.png" /> MeshIO

This is a plugin for [CloudCompare](https://github.com/CloudCompare/CloudCompare) for reading 3D model files (meshes). It uses the [Open Asset Import Library](https://github.com/assimp/assimp) (Assimp).

Currently MeshIO supports reading the following formats:

- [COLLADA](https://en.wikipedia.org/wiki/COLLADA) (\*.dae)
- [glTF](https://en.wikipedia.org/wiki/GlTF) (\*.gltf \*.glb)
- [IFC-SPF](https://en.wikipedia.org/wiki/ISO_10303-21) a.k.a STEP (\*.ifc \*.stp \*.step)

## Building

Unfortunately, because of the way CloudCompare is set up, you have to build all of CloudCompare before you can build a plugin for it. So the first thing to do is to get the [CloudCompare source](https://github.com/CloudCompare/CloudCompare) and [build CloudCompare](https://github.com/CloudCompare/CloudCompare/blob/master/BUILD.md).

Once you have it building properly:

- clone this repository into the `CloudCompare/plugins/private` directory:

      cd CloudCompare/plugins/private
      git clone --recursive https://github.com/asmaloney/MeshIO.git

- re-run CloudCompare's cmake
- turn on `PLUGIN-3rdParty_MESH_IO` in the CloudCompare cmake options
- build CloudCompare

Note: If you are building for _CloudCompare 2.11.x_, use the tagged version of MeshIO at `cloudcompare-2.11.x`

## Caveat

This plugin is using another library to read these file formats. There will be problems with some files (e.g. IFC-SPF is not 100% implemented). It also means that some mesh names might be lost when importing some formats.

Because this plugin is basically translating the Assimp data into something CloudCompare understands, it can be tricky to figure out where the problem lies.

If you find files that do not work as expected, please add an [issue here](https://github.com/asmaloney/MeshIO/issues) and attach the smallest example you have that isn't working. I will try to assess where the problem lies and either report it to the [Assimp](https://github.com/assimp/assimp) project or [CloudCompare](https://github.com/CloudCompare/CloudCompare) project as necessary.

## Icon

The icon is by [qubodup](https://openclipart.org/detail/168258/3d-cube-icon) (Public Domain).

## License

This project is licensed under the BSD 3-Clause license - see [LICENSE](https://github.com/asmaloney/MeshIO/blob/master/LICENSE) file for details.

Individual source files contain the following tag instead of the full license text:

    SPDX-License-Identifier: BSD-3-Clause

This enables machine processing of license information based on [SPDX
License Identifiers](https://spdx.org/ids).

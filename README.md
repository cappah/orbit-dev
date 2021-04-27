# Orbit Engine
# [![Hex.pm](https://img.shields.io/hexpm/l/plug?color=orange&logo=madsycode&logoColor=red&style=plastic)](https://github.com/Madsycode/orbit-dev/blob/2a7f85551977d0d1274db3fe6cd2da9020d23541/LICENSE)
**Orbit** is a multiplatform-focus graphical engine build on top of OpenGl, ImGui and more...
The development of the engine is documented via my youtube channel: [Madsycode](https://www.youtube.com/c/madsycode)

![Orbit engine preview](https://github.com/Madsycode/orbit-dev/blob/master/preview.png)

## Project's Goal
The main goal is to create a friendly community around this engine to shape it, maintain it and potentially make a difference.

## Project Setup
- Orbit has only been tested on Windows platform sofar, allthough the focus is still to make it multiplatform in the future.
- you can clone the repository to your local machine via `git clone https://github.com/Madsycode/orbit-dev.git`
- In the root directory of your cloned repository you will find a `vs2019.bat` file. Execute it to generate a vs-2019 solution.
- you can open the file in an text editor and change the visual studio version to 2017 or any other version. But vs2017 & 2019 are motsly recommended.
- the last important step is to open the **Properties of the CORE-Project** in visual studio and make sure you set the **Precompiled Headers** setting for this file: `CORE/src/Vendor/ImGui/ImGuizmo.cpp` to not **No Using Precompiled Headers**. This should normally be done by premake but it hasn't work for me sofar. so if you can, please fix it.

## Supported features
1. Physical Base Rendering (PBR)
2. ImGui User Interface (Editor)
3. Entity Munipulation via Transform Gizmo
4. Entity Component System (ECS)
5. OpenGL 3.x support
6. Basic XML-Serialization (tinyxml)
7. Event Driven Architechture
8. 3D Model loading with assimp
9. Texture loading via stb_image
10. Point, Spot & Directional lighting
11. ...

## TODOs
1. Invironment map sampling
2. Model animation
3. Scripting support
4. Serialization
5. Shadow kapping
6. User interface
7. Batch renderer
8. Logo for engine
9. ...

## Code of Conduct
Please read [Conduct.md](https://github.com/Madsycode/orbit-dev/blob/master/Conduct.md) for details on our code of conduct.

## Support
Any support, to this project will be highly considered. So you are free to contribute for the future of this engine.

[![Twitter](https://img.shields.io/badge/madsycode--blue.svg?style=social&logo=Twitter)](https://twitter.com/MadsyCode)
[![Youtube](https://img.shields.io/badge/madsycode--red.svg?style=social&logo=youtube)](https://www.youtube.com/c/madsycode)
[![Discord](https://img.shields.io/badge/madsycode%20Server--blue.svg?style=social&logo=Discord)](https://discord.gg/rdRyrcvJ)
[![Patreon](https://img.shields.io/badge/madsycode--green.svg?style=social&logo=Patreon)](https://www.patreon.com/madsycode)

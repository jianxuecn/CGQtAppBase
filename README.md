# CGQtAppBase

## Introduction

This is an example of CG App based on OpenGL and Qt for the *Computer Graphics Course* of School of Engineering Science, University of Chinese Academy of Sciences (UCAS).

It is demonstrated in this project how to start a simple CG related application based on OpenGL and Qt. Following main topics have been included and implemented:

1. Setting up an OpenGL environment (using APIs in modern or traditional style) for scene rendering by Qt;
2. Designing a simple but efficient and extensible framework for organizing scene data for rendering by OpenGL API (partially depending on the scene graph structures of Assimp);
3. Loading scene (object) data from files with different format by Assimp;
4. Calculating necessary transformation parameters for user-scene interaction, including: how to setup projection matrix; how to produce camera matrix according to the position and orientation of the camera; how to rotate the scene object by track ball;
5. Rendering the scene dynamically on a specific frame rate.

## Dependencies

1. **Qt**: Cross-platform Software Development Framework (https://www.qt.io/) under GNU LGPL license (see https://www.qt.io/download-open-source);
2. **Assimp**: Open Asset Import Library (http://assimp.org/ or on [GitHub](https://github.com/assimp/assimp));
3. **glm**: OpenGL Mathematics (https://github.com/g-truc/glm).
4. **OpenGL**: Open Graphics Language, probably the industry's most widely used graphics API (https://www.opengl.org/)

## Instructions for Use

### Prepare develop tools

1. Install Git (optional) to retrieve the source code from GitHub.

2. Install Qt Creator and libraries (Qt 5.15 is preferred) with appropriate C++ compiler (e.g. Clang on macOS, gcc on Linux and MSVC on Windows).
3. Install CMake (https://cmake.org/) for generating native makefiles and workspaces according to the compiler actually used. May be installed together with Qt.

### Prepare dependencies

1. Download, compile and install Assimp by using CMake and underlying compiler. Currently Assimp 5.0.1 is used.
2. Download glm. Because it is a header-only C++ template library, there is no need for compilation.

### Setup local workspace

1. Download the source code of this project.

2. Open CMakeLists.txt in the source directory by Qt Creator, then choose appropriate Qt Kit (with compiler) to setup the local project.

3. Set "`assimp_DIR`" and "`glm_DIR`" in the CMake setting panel of Qt Creator (Projects->Build & Run->Build->Build Settings->CMake->Add->Directory). 
   **Note**: The value of "`assimp_DIR`" and "`glm_DIR`" can also be set by adding a file named "local-dependencies.cmake" to the same directory as CMakeLists.txt, which contains following content and is loaded by CMake during parsing CMakeLists.txt.

   ```cmake
   set(assimp_DIR "/path/to/assimp/lib/cmake/assimp-5.0")
   set(glm_DIR "/path/to/glm/cmake/glm")
   ```

   (Above path should be revised according to your own settings, where the file *-config.cmake or *Config.cmake should be found for setting the corresponding properties for the library.)

### Build the project and have fun!


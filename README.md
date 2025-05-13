# Escher esque Hyperbolic Tiling Creator

A C++ OpenGL application using modern OpenGL, the Eigen library for linear algebra, and stb_image and stb_image_write for image loading and writing.

## 📦 Features

- Modern OpenGL rendering
- Matrix & vector math with Eigen
- Image loading with stb_image
- Image writing with stb_image_write
- GLFW and GLEW for windowing and OpenGL extensions

---

## 🛠️ Requirements

- C++14 or newer
- [CMake](https://cmake.org/) (version 3.10+)
- OpenGL 3.3 compatible GPU and drivers
- Git 
- Visual Studio
- A C++ compiler

---

## 🚀 Building the Project

### Clone the Repository, run CMake and compile the solution

git clone https://github.com/S2ym0d/EscherEsqueHyperbolicTilingCreator.git
cd EscherEsqueHyperbolicTilingCreator
mkdir Build
cd Build
cmake ..

Open the generated .sln file in Visual Studio and compile the solution.

The compiled .exe file can be located in the Build/Debug or Build/Release folder.

### Runtime Requirements

Copy the res folder (found inside the resources/ directory) into the same folder as the generated .exe.

The resources folder contains an images folder with some basic images to start with and a tilingSettings.txt file with template for the tiling settings file.

### The program's user guide

The file Escher_esque_Hyperbolic_Tiling_Creator_User_Guide.pdf contains explanation of the idea the project is built on and the user guide for using the program

## Licenses

This project uses the following third-party libraries:

- **Eigen** - [MPL2 License](https://www.mozilla.org/en-US/MPL/2.0/)
  - See `/licenses/eigen_LICENSE.txt`
- **stb_image** - MIT (https://en.wikipedia.org/wiki/MIT_License)
  - See `/licenses/stb_LICENSE.txt`
- **stb_image_write** - MIT (https://en.wikipedia.org/wiki/MIT_License)
  - See `/licenses/stb_LICENSE.txt`
- **GLEW** - [Modified BSD license](https://glew.sourceforge.net/glew.txt)
  - See `/licenses/glew_LICENSE.txt`
- **GLFW** - [Zlib/libpng license](https://www.glfw.org/license.html)
  - See `/licenses/glfw_LICENSE.txt`

This project itself is licensed under the MIT License. See `LICENSE` for details.

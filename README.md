# Iceberg3D ![Screenshot](http://imgur.com/BKjpnp1.png)

Iceberg3D is an OpenGL based 3D Game Engine.

![Screenshot](http://imgur.com/fGLzSfE.jpg)

## Downloading and Building

First, clone the repo and its submodules:

    git clone --recursive https://github.com/matthewjberger/Iceberg3D
    
Then to build, use [CMake](https://cmake.org/):

    cd Iceberg3D
    mkdir Build
    cd Build
    cmake ..

######Note: When building with Visual Studio, the [Visual C++ Redistributable for Visual Studio 2015](https://www.microsoft.com/en-us/download/details.aspx?id=48145) must be installed. 

## Demos
To build the demos, enable the cmake option:

    ICEBERG_BUILD_DEMOS
    
If using Visual Studio to build the demos, make sure that you set the working directory per demo as follows:

    Right-click on the chosen demo -> Properties -> Debugging -> Working Directory
    
and change to:
    
    $(TargetDir)

## Sandbox

Additionally, there is a bare bones sandbox application that can be used for testing features. To build the sandbox, enable the cmake option:

    ICEBERG_BUILD_SANDBOX
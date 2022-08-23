# caretaker_control

All required C++ libraries are self contained, however, several basic tools are required:

* An installation of Microsoft Visual Studio with the C++ package (Community Edition can be downloaded freely)

* An installation of CMake and Ninja

  * The easiest way to get these is using [Chocolatey](https://chocolatey.org/install)
  * From an elevated Powershell, use `choco install cmake.install --installargs '"ADD_CMAKE_TO_PATH=User"'`
  * and `choco install ninja`
 
After restarting powershell, you should be able to navigate to the cloned repo and run the `.\make_all.bat` script. If all goes well, it should produce a CaretakerControl.exe program file in the build directory.
 
Note that wherever you move CaretakerControl.exe, you must also copy across 'freeglut.dll' and 'glew32.dll' for the program to run.

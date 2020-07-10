cmake_minimum_required(VERSION 3.14.5)
project(tar_prog)

set(CMAKE_CXX_STANDARD 17)
#link_libraries(libc++abi.dylib)


add_executable(tar_prog main.cpp GUI.cpp GUI.h SeveralFiles.cpp SeveralFiles.h File.cpp File.h TarArchive.cpp TarArchive.h
        IArchive.cpp IArchive.h libs/docopt/docopt.h libs/docopt/docopt.cpp)

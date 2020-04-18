cmake_minimum_required(VERSION 3.15)
project(tar_prog)

set(CMAKE_CXX_STANDARD 17)
link_libraries(stdc++fs)

add_executable(tar_prog main.cpp GUI.cpp GUI.h SeveralFiles.cpp SeveralFiles.h File.cpp File.h TarArchive.cpp TarArchive.h IArchive.cpp IArchive.h)
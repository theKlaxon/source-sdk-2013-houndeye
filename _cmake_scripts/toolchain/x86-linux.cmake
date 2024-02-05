set(CMAKE_SYSTEM_NAME "Linux")
set(CMAKE_SYSTEM_PROCESSOR "x86")

set(CMAKE_C_COMPILER_TARGET "x86-linux")
set(CMAKE_CXX_COMPILER_TARGET "x86-linux")

# gcc needs special treatment to target x86
if ( CMAKE_C_COMPILER STREQUAL "gcc" OR CMAKE_CXX_COMPILER STREQUAL "g++" )
	set(CMAKE_C_COMPILER gcc -m32)
	set(CMAKE_CXX_COMPILER g++ -m32)
endif ()

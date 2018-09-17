# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.4.1)
#设置生成的so动态库最后输出的路径
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/src/main/jniLibs/${ANDROID_ABI})

# Specifies a path to native header files.
#include_directories(src/main/cpp/ src/main/cpp/res/)

#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=undefined")
# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.

add_library( # Sets the name of the library.
             native-lib
             # Sets the library as a shared library.
             SHARED
             # Provides a relative path to your source file(s).
             src/main/cpp/native-lib.cpp )

#设置根路径
set(SRC_ROOT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/src/main/cpp)

#添加查找头文件的路径
#include_directories(src/main/cpp/ src/main/cpp/res/)
include_directories(${SRC_ROOT_PATH} ${SRC_ROOT_PATH}/aes)

#获取文件
file(GLOB CPP_FILES_IN_JNI "${SRC_ROOT_PATH}/*.c" "${SRC_ROOT_PATH}/aes/*.c")

add_library( data_aes
             SHARED
             ${CPP_FILES_IN_JNI} )

# Searches for a specified prebuilt library and stores the path as a
# variable. Because CMake includes system libraries in the search path by
# default, you only need to specify the name of the public NDK library
# you want to add. CMake verifies that the library exists before
# completing its build.

find_library( # Sets the name of the path variable.
              log-lib
              # Specifies the name of the NDK library that
              # you want CMake to locate.
              log )

# Specifies libraries CMake should link to your target library. You
# can link multiple libraries, such as libraries you define in this
# build script, prebuilt third-party libraries, or system libraries.

target_link_libraries( # Specifies the target library.
                       native-lib
                       # Links the target library to the log library
                       # included in the NDK.
                       ${log-lib} )


target_link_libraries( # Specifies the target library.
                       data_aes
                       # Links the target library to the log library
                       # included in the NDK.
                       ${log-lib} )



cmake_minimum_required(VERSION 3.21)
# 设置NDK路径
set(NDK_PATH D:/android-ndk-r26b)

##################### Android设置 #####################
set(CMAKE_SYSTEM_NAME ANDROID) # 设置目标编译平台参数 Android
set(CMAKE_BUILD_TYPE Release)
set(CMAKE_SYSTEM_VERSION 21) # 系统版本
set(ANDROID_PLATFORM 21) # 平台版本
set(ANDROID_SDK_TOOLS_VERSION 20) # 平台版本
set(ANDROID_ABI arm64-v8a) # 设置目标构架 armeabi-v7a arm64-v8a x86 x86_64
set(ANDROID_NDK ${NDK_PATH}) # 设置ndk路径
set(CMAKE_TOOLCHAIN_FILE ${NDK_PATH}/build/cmake/android.toolchain.cmake)
set(ANDROID_SDK_ROOT ${NDK_PATH})

project(NativeSurface)

set(CMAKE_CXX_STANDARD 17)

##################### CMake头文件设置 #####################
include_directories( # 设置全局头文件目录 使其他源码文件可在任意目录<头文件.h>
        include/
        include/ImGui
        include/Android_touch
        include/Android_shm
        include/native_surface
        include/Android_draw
        include/widget
        include/stb_image
        include/tool
)
##################### CMake源文件设置 #####################
FILE(GLOB_RECURSE FILE_SOURCES src/*.c*)
##################### 添加产物 #####################
add_executable(NativeSurface ${FILE_SOURCES})
##################### 连接库文件 #####################
target_link_libraries(NativeSurface PRIVATE EGL GLESv3 log android GLESv2)

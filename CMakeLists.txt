cmake_minimum_required(VERSION 3.10)
project(TerrariaFake)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB LIBS_SOURCE_FILES libsSource/*.cpp)
file(GLOB SOURCE_FILES src/*.cpp)


# Find raylib package if installed globally, otherwise set the path to Raylib manually
find_package(raylib 4.0 QUIET) # Look for Raylib 4.0 or later

# If Raylib wasn't found, download and build it from source
if (NOT raylib_FOUND)
    include(FetchContent)
    FetchContent_Declare(
        raylib
        GIT_REPOSITORY https://github.com/raysan5/raylib.git
        GIT_TAG 4.5.0 # You can set a specific tag here
    )
    FetchContent_MakeAvailable(raylib)
endif()

include_directories(include libs/)

# Add executable and link Raylib
add_executable(TerrariaFake
  ${SOURCE_FILES}
  ${LIBS_SOURCE_FILES}
)
target_link_libraries(TerrariaFake raylib)

# For custom paths to Raylib, uncomment the following and set RAYLIB_DIR to your path
# set(RAYLIB_DIR "/path/to/raylib")
# target_include_directories(raylib_project PRIVATE ${RAYLIB_DIR}/include)
# target_link_libraries(raylib_project ${RAYLIB_DIR}/lib/libraylib.a) # Adjust for library file location

# Additional configurations for cross-platform compatibility
if (APPLE)
    target_link_libraries(TerrariaFake "-framework OpenGL" "-framework Cocoa" "-framework IOKit" "-framework CoreVideo")
elseif (UNIX AND NOT APPLE)
    target_link_libraries(TerrariaFake pthread GL dl m)
endif()

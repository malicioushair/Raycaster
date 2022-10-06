cmake_minimum_required(VERSION 3.21)
project(Raycaster LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (MSVC)
    add_compile_options(/W4 /WX)
endif()

if(NOT EXISTS "${CMAKE_BINARY_DIR}/conan.cmake")
     message(STATUS "Downloading conan.cmake from https://github.com/conan-io/cmake-conan")
     file(DOWNLOAD "https://raw.githubusercontent.com/conan-io/cmake-conan/master/conan.cmake"
     "${CMAKE_BINARY_DIR}/conan.cmake")
endif()

include(${CMAKE_BINARY_DIR}/conan.cmake)
conan_cmake_run(
     CONANFILE conanfile.txt
     BASIC_SETUP
     BUILD missing
)

file(GLOB SRC_FILES
     "src/*.h"
     "src/*.cpp"
)

add_executable(${PROJECT_NAME} ${SRC_FILES})
target_link_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_BINARY_DIR}/bin)
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
target_link_libraries(${PROJECT_NAME} ${CONAN_LIBS})
set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "/ignore:4099")

if (WIN32 AND BUILD_SHARED_LIBS)
    add_custom_command(TARGET Raycaster POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:Raycaster> $<TARGET_FILE_DIR:Raycaster> COMMAND_EXPAND_LISTS)
endif()

install(TARGETS Raycaster)

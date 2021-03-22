#Change this if you need to target a specific CMake version
cmake_minimum_required(VERSION 3.4)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake_modules)

# Enable debug symbols by default
# must be done before project() statement
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build (Debug or Release)" FORCE)
endif()

project(gameboy)
include(add_sources)

add_definitions(-O3 -std=c++14)

add_subdirectory(src)

declare_executable(gameboy)

# Detect and add SFML
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})

#Find any version 2.X of SFML
#See the FindSFML.cmake file for additional details and instructions
find_package(SFML 2 REQUIRED network audio graphics window system)
if(SFML_FOUND)
  include_directories(${SFML_INCLUDE_DIR})
  #target_link_libraries(${EXECUTABLE_NAME} ${SFML_LIBRARIES} ${SFML_DEPENDENCIES})
  target_link_libraries(gameboy ${SFML_LIBRARIES} ${SFML_DEPENDENCIES})
endif()


# CPack packaging
include(InstallRequiredSystemLibraries)
set(CPACK_PACKAGE_VERSION_MAJOR "${gameboy_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${gameboy_VERSION_MINOR}")
include(CPack)

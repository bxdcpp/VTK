
cmake_minimum_required(VERSION 3.12 FATAL_ERROR)

project(BoxWidget2)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../../../bin)
set(VTK_DIR S:/VTK-build)
set(ITK_DIR S:/ITK-build)

find_package(ITK REQUIRED)

IF(ITK_FOUND) 
    INCLUDE(${ITK_USE_FILE})

ELSE(ITK_FOUND) 
    MESSAGE(FATAL_ERROR 
            "ITK not found. Please set ITK_DIR.") 
ENDIF(ITK_FOUND)
#message("ITK_USE_FILE BoxWidget2: ${ITK_USE_FILE}")

find_package(VTK REQUIRED)

if (NOT VTK_FOUND)
  message("Skipping BoxWidget2: ${VTK_NOT_FOUND_MESSAGE}")
  return()
endif()
message (STATUS "VTK_VERSION: ${VTK_VERSION}")
if (VTK_VERSION VERSION_LESS "8.90.0")
  # old system
  include(${VTK_USE_FILE})
  add_executable(BoxWidget2 MACOSX_BUNDLE BoxWidget2.cxx )
  target_link_libraries(BoxWidget2 PRIVATE ${VTK_LIBRARIES})
else()
  # Prevent a "command line is too long" failure in Windows.
  set(CMAKE_NINJA_FORCE_RESPONSE_FILE "ON" CACHE BOOL "Force Ninja to use response files.")
  add_executable(BoxWidget2 MACOSX_BUNDLE BoxWidget2.cxx )
  target_link_libraries(BoxWidget2 PRIVATE ${VTK_LIBRARIES} ${ITK_LIBRARIES})
  # vtk_module_autoinit is needed
  vtk_module_autoinit(
    TARGETS BoxWidget2
    MODULES ${VTK_LIBRARIES}
    )
endif()

cmake_minimum_required(VERSION 3.3 FATAL_ERROR)

project(AngleWidget)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../../../bin)
set(VTK_DIR S:/VTK-build)
set(ITK_DIR S:/ITK-build)

find_package(VTK COMPONENTS 
  vtkCommonColor
  vtkCommonCore
  vtkCommonDataModel
  vtkFiltersSources
  vtkInteractionStyle
  vtkInteractionWidgets
  vtkRenderingContextOpenGL2
  vtkRenderingCore
  vtkRenderingFreeType
  vtkRenderingGL2PSOpenGL2
  vtkRenderingOpenGL2
  QUIET
)

if (NOT VTK_FOUND)
  message("Skipping AngleWidget: ${VTK_NOT_FOUND_MESSAGE}")
  return ()
endif()
message (STATUS "VTK_VERSION: ${VTK_VERSION}")
if (VTK_VERSION VERSION_LESS "8.90.0")
  # old system
  include(${VTK_USE_FILE})
  add_executable(AngleWidget MACOSX_BUNDLE AngleWidget.cxx )
  target_link_libraries(AngleWidget PRIVATE ${VTK_LIBRARIES})
else ()
  # include all components
  add_executable(AngleWidget MACOSX_BUNDLE AngleWidget.cxx )
  target_link_libraries(AngleWidget PRIVATE ${VTK_LIBRARIES})
  # vtk_module_autoinit is needed
  vtk_module_autoinit(
    TARGETS AngleWidget
    MODULES ${VTK_LIBRARIES}
    )
endif ()
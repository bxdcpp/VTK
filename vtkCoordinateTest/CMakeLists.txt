cmake_minimum_required(VERSION 3.3 FATAL_ERROR)

project(vtkCoordinateTest)

find_package(VTK COMPONENTS 
  vtkCommonColor
  vtkCommonCore
  vtkCommonDataModel
  vtkFiltersGeneral
  vtkFiltersSources
  vtkInteractionStyle
  vtkRenderingContextOpenGL2
  vtkRenderingCore
  vtkRenderingFreeType
  vtkRenderingGL2PSOpenGL2
  vtkRenderingOpenGL2
  QUIET
)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../bin)

if (NOT VTK_FOUND)
  message("Skipping vtkCoordinateTest: ${VTK_NOT_FOUND_MESSAGE}")
  return ()
endif()
message (STATUS "VTK_VERSION: ${VTK_VERSION}")
if (VTK_VERSION VERSION_LESS "8.90.0")
  # old system
  include(${VTK_USE_FILE})
  add_executable(vtkCoordinateTest MACOSX_BUNDLE vtkCoordinateTest.cxx )
  target_link_libraries(Cursor2D PRIVATE ${VTK_LIBRARIES})
else ()
  # include all components
  add_executable(vtkCoordinateTest MACOSX_BUNDLE vtkCoordinateTest.cxx )
  target_link_libraries(vtkCoordinateTest PRIVATE ${VTK_LIBRARIES})
  # vtk_module_autoinit is needed
  vtk_module_autoinit(
    TARGETS vtkCoordinateTest
    MODULES ${VTK_LIBRARIES}
    )
endif ()
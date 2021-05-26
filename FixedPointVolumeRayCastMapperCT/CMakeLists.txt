cmake_minimum_required(VERSION 3.3 FATAL_ERROR)

project(FixedPointVolumeRayCastMapperCT)

find_package(VTK COMPONENTS 
  vtkCommonColor
  vtkCommonCore
  vtkCommonDataModel
  vtkIOImage
  vtkIOXML
  vtkImagingCore
  vtkInteractionStyle
  vtkInteractionWidgets
  vtkRenderingContextOpenGL2
  vtkRenderingCore
  vtkRenderingFreeType
  vtkRenderingGL2PSOpenGL2
  vtkRenderingOpenGL2
  vtkRenderingVolume
  vtkRenderingVolumeOpenGL2
  QUIET
)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../bin)
if (NOT VTK_FOUND)
  message("Skipping FixedPointVolumeRayCastMapperCT: ${VTK_NOT_FOUND_MESSAGE}")
  return ()
endif()
message (STATUS "VTK_VERSION: ${VTK_VERSION}")
if (VTK_VERSION VERSION_LESS "8.90.0")
  # old system
  include(${VTK_USE_FILE})
  add_executable(FixedPointVolumeRayCastMapperCT MACOSX_BUNDLE FixedPointVolumeRayCastMapperCT.cxx )
  target_link_libraries(FixedPointVolumeRayCastMapperCT PRIVATE ${VTK_LIBRARIES})
else ()
  # include all components
  add_executable(FixedPointVolumeRayCastMapperCT MACOSX_BUNDLE FixedPointVolumeRayCastMapperCT.cxx )
  target_link_libraries(FixedPointVolumeRayCastMapperCT PRIVATE ${VTK_LIBRARIES})
  # vtk_module_autoinit is needed
  vtk_module_autoinit(
    TARGETS FixedPointVolumeRayCastMapperCT
    MODULES ${VTK_LIBRARIES}
    )
endif ()
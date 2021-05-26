cmake_minimum_required(VERSION 3.3 FATAL_ERROR)

project(ImageShrink3D)

find_package(VTK COMPONENTS 
  vtkCommonCore
  vtkCommonDataModel
  vtkImagingCore
  vtkImagingSources
  vtkInteractionStyle
  vtkRenderingContextOpenGL2
  vtkRenderingCore
  vtkRenderingFreeType
  vtkRenderingGL2PSOpenGL2
  vtkRenderingOpenGL2 QUIET)
if (NOT VTK_FOUND)
  message("Skipping ImageShrink3D: ${VTK_NOT_FOUND_MESSAGE}")
  return ()
endif()
message (STATUS "VTK_VERSION: ${VTK_VERSION}")
if (VTK_VERSION VERSION_LESS "8.90.0")
  # old system
  include(${VTK_USE_FILE})
  add_executable(ImageShrink3D MACOSX_BUNDLE ImageShrink3D.cxx )
  target_link_libraries(ImageShrink3D PRIVATE ${VTK_LIBRARIES})
else ()
  # include all components
  add_executable(ImageShrink3D MACOSX_BUNDLE ImageShrink3D.cxx )
  target_link_libraries(ImageShrink3D PRIVATE ${VTK_LIBRARIES})
  # vtk_module_autoinit is needed
  vtk_module_autoinit(
    TARGETS ImageShrink3D
    MODULES ${VTK_LIBRARIES}
    )
endif () 
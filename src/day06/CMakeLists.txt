get_filename_component(targetName ${CMAKE_CURRENT_SOURCE_DIR} NAME)
add_executable(${targetName} 
	main.cpp
)
target_link_libraries(${targetName} 
  PUBLIC project_options project_warnings
  PRIVATE fmt::fmt spdlog::spdlog Microsoft.GSL::GSL range-v3::range-v3 common
)

add_custom_target(${targetName}-copy-data ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/data ${CMAKE_CURRENT_BINARY_DIR}/data
    DEPENDS ${targetName} )
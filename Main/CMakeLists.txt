cmake_minimum_required(VERSION 3.12)

project(main)

add_executable(main WIN32
	main.cpp
)

target_link_libraries(main PRIVATE Graphic)

set_target_properties(main PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/)

add_custom_command(TARGET main POST_BUILD
					COMMAND ${CMAKE_COMMAND} -E copy_directory
					${CMAKE_SOURCE_DIR}/res $<TARGET_FILE_DIR:main>/res)
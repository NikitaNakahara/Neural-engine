cmake_minimum_required(VERSION 3.12)

project(Graphic)

set(ALL_SRC
	Application.cpp
	Application.hpp
	Log.hpp
	render/OpenGL/Renderer.cpp
	render/OpenGL/Renderer.hpp
	render/OpenGL/Camera.cpp
	render/OpenGL/Camera.hpp
	render/OpenGL/Framebuffer.cpp
	render/OpenGL/Framebuffer.hpp
	render/OpenGL/shader/ShaderProgram.hpp
	render/OpenGL/shader/ShaderProgram.cpp
	render/OpenGL/shader/ShaderReader.hpp
	render/OpenGL/shader/ShaderReader.cpp
	render/OpenGL/texture/TextureInit.hpp
	render/OpenGL/texture/TextureInit.cpp
	render/OpenGL/texture/skyboxTexture.hpp
	render/OpenGL/texture/skyboxTexture.cpp
	render/OpenGL/texture/stb_image.cpp
	render/OpenGL/texture/stb_image.h
	render/OpenGL/models/ModelLoader.cpp
	render/OpenGL/models/ModelLoader.hpp
	render/OpenGL/models/MeshLoader.cpp
	render/OpenGL/models/MeshLoader.hpp
	render/OpenGL/FileLoader.cpp
	render/OpenGL/FileLoader.hpp
)

add_library(Graphic
	${ALL_SRC}
)

add_subdirectory(../external/glfw ${CMAKE_CURRENT_BINARY_DIR}/glfw)
target_link_libraries(Graphic PRIVATE glfw)

add_subdirectory(../external/glad ${CMAKE_CURRENT_BINARY_DIR}/glad)
target_link_libraries(Graphic PRIVATE glad)

add_subdirectory(../external/spdlog ${CMAKE_CURRENT_BINARY_DIR}/spdlog)
target_link_libraries(Graphic PRIVATE spdlog)

add_subdirectory(../external/glm ${CMAKE_CURRENT_BINARY_DIR}/glm)
target_link_libraries(Graphic PRIVATE glm)

add_subdirectory(../external/assimp ${CMAKE_CURRENT_BINARY_DIR}/assimp)
target_link_libraries(Graphic PRIVATE assimp)


set(IMGUI_INCLUDES
	../external/imgui/imgui.h
	../external/imgui/backends/imgui_impl_glfw.h
	../external/imgui/backends/imgui_impl_opengl3.h
)

set(IMGUI_SOURCES
	../external/imgui/imgui.cpp
	../external/imgui/backends/imgui_impl_glfw.cpp
	../external/imgui/backends/imgui_impl_opengl3.cpp
	../external/imgui/imgui_demo.cpp
	../external/imgui/imgui_tables.cpp
	../external/imgui/imgui_widgets.cpp
	../external/imgui/imgui_draw.cpp
)

add_library(ImGui STATIC
	${IMGUI_INCLUDES}
	${IMGUI_SOURCES}
)

target_include_directories(ImGui PUBLIC ../external)
target_include_directories(ImGui PUBLIC ../external/imgui)
target_link_libraries(ImGui PRIVATE glad glfw)

target_link_libraries(Graphic PRIVATE ImGui)
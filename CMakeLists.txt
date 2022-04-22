cmake_minimum_required(VERSION 3.2 FATAL_ERROR)

project(DH2323_Rendering VERSION 1.0
  DESCRIPTION "Rendering track of DH2323 - Computer Graphics and Interaction"
  LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

message("Processor: ${CMAKE_SYSTEM_PROCESSOR}")
message("Voidptr size: ${CMAKE_SIZEOF_VOID_P}") # 8 for 64 bit and 4 for 32 bit
message("Binary dir: ${CMAKE_CURRENT_BINARY_DIR}") 

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "bin")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "lib")
set(CMAKE_EXTERNAL_DIRECTORY       "external")

set(SUBMODULE_DIR ${CMAKE_SOURCE_DIR}/submodules)
add_subdirectory(${SUBMODULE_DIR}/glfw)
add_subdirectory(${SUBMODULE_DIR}/glm)
add_subdirectory(${SUBMODULE_DIR}/glad)

set(LABSCENES_DIR ${CMAKE_SOURCE_DIR}/labscenes)

include_directories(
  ${CMAKE_SOURCE_DIR}/include
  ${LABSCENES_DIR}
  ${GLFW_INCLUDE_DIRS}
  ${GLM_INCLUDE_DIR}
  ${GLAD_INCLUDE_DIRS}
  ${SUBMODULE_DIR}/imgui
)

file(GLOB SRCFILES ${CMAKE_SOURCE_DIR}/src/*.cpp)
file(GLOB SCENEFILES ${LABSCENES_DIR}/*.cpp ${LABSCENES_DIR}/*.h)
file(GLOB INCLUDEHEADERS ${CMAKE_SOURCE_DIR}/include/*.h)
file(GLOB IMGUI 
  ${SUBMODULE_DIR}/imgui/imgui.cpp 
  ${SUBMODULE_DIR}/imgui/imgui_draw.cpp
  ${SUBMODULE_DIR}/imgui/imgui_tables.cpp
  ${SUBMODULE_DIR}/imgui/imgui_widgets.cpp
  ${SUBMODULE_DIR}/imgui/backends/imgui_impl_opengl3.cpp 
  ${SUBMODULE_DIR}/imgui/backends/imgui_impl_glfw.cpp
)

add_executable(${PROJECT_NAME} ${SRCFILES} ${SCENEFILES} ${INCLUDEHEADERS} ${IMGUI})

target_link_libraries(${PROJECT_NAME} glm glfw glad)
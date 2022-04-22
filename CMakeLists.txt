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

include_directories(
  ${CMAKE_SOURCE_DIR}/include
  ${GLFW_INCLUDE_DIRS}
  ${GLM_INCLUDE_DIR}
  ${GLAD_INCLUDE_DIRS}
)

file(GLOB SRCFILES ${CMAKE_SOURCE_DIR}/src/*.cpp)

add_executable(${PROJECT_NAME} ${SRCFILES})

target_link_libraries(${PROJECT_NAME} glm glfw glad)
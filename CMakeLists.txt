cmake_minimum_required(VERSION 4.4.2)
project(CPURasterizer)

set(CMAKE_CXX_STANDARD 23)

file(GLOB_RECURSE RASTERIZER_HEADERS "include/*.hpp")
file(GLOB_RECURSE RASTERIZER_SOURCES "source/*.cpp")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")

add_subdirectory(vendored/SDL EXCLUDE_FROM_ALL)

add_executable(CPURasterizer
	${RASTERIZER_HEADERS}
	${RASTERIZER_SOURCES}
)

target_include_directories(CPURasterizer PUBLIC
	"${CMAKE_CURRENT_SOURCE_DIR}/include"
)

target_link_libraries(CPURasterizer 
	PRIVATE SDL3::SDL3
)
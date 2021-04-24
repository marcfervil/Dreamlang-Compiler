set (CMAKE_CXX_STANDARD 17)

cmake_minimum_required(VERSION 3.13)
project(dreamlang)


include_directories(dreamlang ${CMAKE_CURRENT_SOURCE_DIR}/../RuntimeLib/include)

set(stdlib "${CMAKE_CURRENT_SOURCE_DIR}/../RuntimeLib")

# Create shared library
file(GLOB SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/../RuntimeLib/StandardLib/*.cpp)
file(GLOB SOURCES2 ${CMAKE_CURRENT_SOURCE_DIR}/../RuntimeLib/*.cpp)
add_library(dreamlang SHARED ${SOURCES} ${SOURCES2})



# Install library
#install(TARGETS dreamlang DESTINATION lib/${PROJECT_NAME})




project(compiler)

find_package(LLVM REQUIRED CONFIG)

include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

llvm_map_components_to_libnames(llvm_libs support core irreader)
#target_link_libraries(simple-tool ${llvm_libs})

include_directories(compiler ${CMAKE_CURRENT_SOURCE_DIR}/../Compiler/include)
file(GLOB SOURCES3 ${CMAKE_CURRENT_SOURCE_DIR}/../Compiler/*.cpp)
add_library(compiler SHARED ${SOURCES3})

llvm_map_components_to_libnames(llvm_libs
	support
	core
	target
	ExecutionEngine
	Object
	irreader
)


# Link against LLVM libraries
target_link_libraries(compiler ${llvm_libs} LLVM dreamlang)

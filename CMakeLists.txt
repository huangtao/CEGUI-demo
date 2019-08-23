# CMake版本要求
cmake_minimum_required(VERSION 3.10)

# 项目名称
project(CEGUIDemo)

MESSAGE(STATUS "operation system is ${CMAKE_SYSTEM}")

# c++11
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
	if (NOT CMAKE_BUILD_TYPE)
		set(CMAKE_BUILD_TYPE "Release")
	endif()

	add_compile_options(-g)
	add_compile_options(-O2)
	add_compile_options(-Wall -Wno-unused-parameter)
	# c语言没有c++11,所以用CMAKE_CXX_FLAGS
	set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
elseif(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
	# VC默认不支持无BOM中文编码
	if(MSVC)
    	add_definitions(-D_CRT_SECURE_NO_WARNINGS)
    	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /source-charset:utf-8")
	endif()
	# 默认构建发布版本
	if(NOT CMAKE_BUILD_TYPE)
  		set(CMAKE_BUILD_TYPE "RelWithDebInfo")
	endif()

	add_compile_options(/W4)
endif()

# 头文件目录和库目录
include_directories (
	d:/CEGUI/cegui-0.8.7/dependencies/include
	d:/CEGUI/cegui-0.8.7/cegui/include
	d:/CEGUI/cegui-0.8.7/build/cegui/include
)
link_directories (
	d:/CEGUI/cegui-0.8.7/dependencies/lib
	d:/CEGUI/cegui-0.8.7/dependencies/lib/dynamic
	d:/CEGUI/cegui-0.8.7/dependencies/lib/static
	d:/CEGUI/cegui-0.8.7/build/lib
)

# 输出到bin目录
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin) 

# 包含子目录
add_subdirectory(helloworld)

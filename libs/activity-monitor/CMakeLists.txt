cmake_minimum_required(VERSION 3.2)
project(activity-monitor VERSION 1.0)

add_library(activity-monitor SHARED dllmain.cpp)

# Common compilation defines/options
if(MSVC)
	target_compile_options(activity-monitor PRIVATE /W4)
	string(REGEX REPLACE "/W3" "" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS}) # remove /W3, because we add /W4

	# CRT - multithreaded dll
	target_compile_options(activity-monitor PRIVATE $<$<CONFIG:DEBUG>:/MDd>)
	target_compile_options(activity-monitor PRIVATE $<$<CONFIG:RELEASE>:/MD>)
else()
	target_compile_options(activity-monitor PRIVATE -Wall -Wextra -Wpedantic)
endif()

target_compile_definitions(activity-monitor PRIVATE -DDLL_EXPORTS)


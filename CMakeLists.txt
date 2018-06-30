cmake_minimum_required(VERSION 3.2)

add_subdirectory("libs/pugixml")
add_subdirectory("libs/activity-monitor")

project(EyeLeo VERSION 1.3.3)

set(SOURCE_FILES_FOLDER ${CMAKE_CURRENT_SOURCE_DIR}/source/code)
add_executable(EyeLeo ${SOURCE_FILES_FOLDER}/main.cpp)

add_dependencies(EyeLeo activity-monitor)

# Common compilation defines/options
if(MSVC)
	target_compile_definitions(EyeLeo PRIVATE
		-D_CRT_SECURE_NO_WARNINGS
		-D_UNICODE
		-DUNICODE)
endif()

if(MSVC)
	target_compile_options(EyeLeo PRIVATE /W4)
else()
	target_compile_options(EyeLeo PRIVATE -Wall -Wextra -Wpedantic)
endif()

# CRT - multithreaded dll
target_compile_options(EyeLeo PRIVATE $<$<CONFIG:DEBUG>:/MDd>)
target_compile_options(EyeLeo PRIVATE $<$<CONFIG:RELEASE>:/MD>)

# wxWidgets dependency
if(NOT DEFINED ENV{WXWIN})
	message(FATAL_ERROR "WXWIN environment variable is not set")
endif()

target_include_directories(EyeLeo PRIVATE "$ENV{WXWIN}/include")
target_compile_definitions(EyeLeo PRIVATE
	-D__WXMSW__
	-DwxUSE_TASKBARICON_BALLOONS)

target_compile_definitions(EyeLeo PRIVATE
		-D$<$<CONFIG:DEBUG>:__WXDEBUG__>)

# wxWidgets debug libs
foreach(LIB_NAME wxbase30ud wxmsw30ud_core wxmsw30ud_adv wxtiffd wxjpegd wxpngd wxzlibd)
	find_library(LIB_PATH ${LIB_NAME} HINTS $ENV{WXWIN}/lib/vc_lib)
	if(NOT LIB_PATH)
		message(FATAL_ERROR "Can't find wxWidgets library ${LIB_NAME}")
	endif()
	target_link_libraries(EyeLeo debug ${LIB_PATH})
	unset(LIB_PATH CACHE)
endforeach()

# wxWidgets release libs
foreach(LIB_NAME wxbase30u wxmsw30u_core wxmsw30u_adv wxtiff wxjpeg wxpng wxzlib)
	find_library(LIB_PATH ${LIB_NAME} HINTS $ENV{WXWIN}/lib/vc_lib)
	if(NOT LIB_PATH)
		message(FATAL_ERROR "Can't find wxWidgets library ${LIB_NAME}")
	endif()
	target_link_libraries(EyeLeo optimized ${LIB_PATH})
	unset(LIB_PATH CACHE)
endforeach()

# PugiXml dependency
target_include_directories(EyeLeo PRIVATE "libs/pugixml/src/")

target_compile_definitions(EyeLeo PRIVATE
	-DPUGIXML_WCHAR_MODE)

target_link_libraries(EyeLeo ${PROJECT_BINARY_DIR}/libs/pugixml/${CMAKE_CFG_INTDIR}/pugixml.lib)
add_dependencies(EyeLeo pugixml)

target_compile_definitions(pugixml PUBLIC
	-DPUGIXML_WCHAR_MODE)

# Source files
target_sources(EyeLeo PRIVATE 
	${SOURCE_FILES_FOLDER}/activity_monitor.cpp
	${SOURCE_FILES_FOLDER}/activity_monitor.h
	${SOURCE_FILES_FOLDER}/beforepause_wnd.cpp
	${SOURCE_FILES_FOLDER}/beforepause_wnd.h
	${SOURCE_FILES_FOLDER}/bigpause_wnd.cpp
	${SOURCE_FILES_FOLDER}/bigpause_wnd.h
	${SOURCE_FILES_FOLDER}/debug_wnd.cpp
	${SOURCE_FILES_FOLDER}/debug_wnd.h
	${SOURCE_FILES_FOLDER}/excercises.cpp
	${SOURCE_FILES_FOLDER}/excercises.h
	${SOURCE_FILES_FOLDER}/image_resources.cpp
	${SOURCE_FILES_FOLDER}/image_resources.h
	${SOURCE_FILES_FOLDER}/language_set.cpp
	${SOURCE_FILES_FOLDER}/language_set.h
	${SOURCE_FILES_FOLDER}/logging.cpp
	${SOURCE_FILES_FOLDER}/logging.h
	${SOURCE_FILES_FOLDER}/main.h
	${SOURCE_FILES_FOLDER}/minipause_wnd.cpp
	${SOURCE_FILES_FOLDER}/minipause_wnd.h
	${SOURCE_FILES_FOLDER}/notification_wnd.cpp
	${SOURCE_FILES_FOLDER}/notification_wnd.h
	${SOURCE_FILES_FOLDER}/oscapabilities.cpp
	${SOURCE_FILES_FOLDER}/oscapabilities.h
	${SOURCE_FILES_FOLDER}/settings.cpp
	${SOURCE_FILES_FOLDER}/settings.h
	${SOURCE_FILES_FOLDER}/settings_wnd.cpp
	${SOURCE_FILES_FOLDER}/settings_wnd.h
	${SOURCE_FILES_FOLDER}/task_mgr.cpp
	${SOURCE_FILES_FOLDER}/task_mgr.h
	${SOURCE_FILES_FOLDER}/timeloc.cpp
	${SOURCE_FILES_FOLDER}/timeloc.h
	${SOURCE_FILES_FOLDER}/waiting_wnd.cpp
	${SOURCE_FILES_FOLDER}/waiting_wnd.h)

target_link_libraries(EyeLeo 
	comctl32.lib
	winmm.lib
	rpcrt4.lib
	Wtsapi32.lib)

if(MSVC)
	set(CMAKE_EXE_LINKER_FLAGS
		"${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS")
endif()

# post-build copy
add_custom_command(TARGET EyeLeo POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy 
	${CMAKE_BINARY_DIR}/$<CONFIGURATION>/EyeLeo.exe ${CMAKE_SOURCE_DIR}/bin/EyeLeo.exe
	COMMAND ${CMAKE_COMMAND} -E copy
	${CMAKE_BINARY_DIR}/libs/activity-monitor/$<CONFIGURATION>/activity-monitor.dll ${CMAKE_SOURCE_DIR}/bin/activity-monitor.dll)

# set icon
if(WIN32)
	add_custom_command(TARGET EyeLeo POST_BUILD
		COMMAND ${CMAKE_SOURCE_DIR}/tools/ReplaceVistaIcon.exe 
		${CMAKE_SOURCE_DIR}/bin/EyeLeo.exe ${CMAKE_SOURCE_DIR}/source/resource/icon.ico)
endif()
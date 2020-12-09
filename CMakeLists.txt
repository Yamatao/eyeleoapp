cmake_minimum_required(VERSION 3.2)

if(POLICY CMP0074)
    cmake_policy(SET CMP0074 NEW)
endif()

add_subdirectory("libs/pugixml")

project(EyeLeo VERSION 1.3.6)

set(SOURCE_FILES_FOLDER ${CMAKE_CURRENT_SOURCE_DIR}/source/code)
add_executable(EyeLeo WIN32 ${SOURCE_FILES_FOLDER}/main.cpp)

if(WIN32)
    add_subdirectory("libs/activity-monitor")
    add_dependencies(EyeLeo activity-monitor)
endif()

# Common compilation defines/options
if(MSVC)
    target_compile_definitions(EyeLeo PRIVATE
        -D_CRT_SECURE_NO_WARNINGS
        -D_UNICODE
        -DUNICODE)

    target_compile_options(EyeLeo PRIVATE /W4)
    string(REGEX REPLACE "/W3" "" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS}) # remove /W3, because we add /W4

    # Multithreaded dll CRT
    target_compile_options(EyeLeo PRIVATE $<$<CONFIG:DEBUG>:/MDd>)
    target_compile_options(EyeLeo PRIVATE $<$<CONFIG:RELEASE>:/MD>)
else()
    target_compile_options(EyeLeo PRIVATE -Wall -Wextra -Wpedantic)
endif()

set_target_properties(EyeLeo PROPERTIES
        CXX_STANDARD 14
        CXX_STANDARD_REQUIRED ON)

# wxWidgets dependency
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    set(wxWidgets_USE_DEBUG OFF)
endif()
find_package(wxWidgets REQUIRED COMPONENTS core base adv)

target_include_directories(EyeLeo PRIVATE ${wxWidgets_INCLUDE_DIRS})
target_link_libraries(EyeLeo PRIVATE ${wxWidgets_LIBRARIES})

target_compile_definitions(EyeLeo PRIVATE
        -D__WXMSW__
        -DwxUSE_TASKBARICON_BALLOONS)

target_compile_definitions(EyeLeo PRIVATE
        -D$<$<CONFIG:DEBUG>:__WXDEBUG__>)

# PugiXml dependency
target_link_libraries(EyeLeo PRIVATE pugixml)

target_compile_definitions(pugixml PUBLIC
        -DPUGIXML_WCHAR_MODE)

# Source files
target_sources(EyeLeo PRIVATE 
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

if(WIN32)
    target_sources(EyeLeo PRIVATE
        ${SOURCE_FILES_FOLDER}/activity_monitor.cpp
        ${SOURCE_FILES_FOLDER}/activity_monitor.h)

    target_link_libraries(EyeLeo PRIVATE
        comctl32.lib
        winmm.lib
        rpcrt4.lib
        Wtsapi32.lib)
endif()

if(WIN32)
    set_property(GLOBAL PROPERTY TARGET_FORMAT "exe")
elseif(UNIX)
    set_property(GLOBAL PROPERTY TARGET_FORMAT "so")
endif()

# post-build copy
add_custom_command(TARGET EyeLeo POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
    $<TARGET_FILE:EyeLeo> ${CMAKE_SOURCE_DIR}/bin/eyeleo.${TARGET_FORMAT})

# post-build activity-monitor copy
if(WIN32)
    add_custom_command(TARGET EyeLeo POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        $<TARGET_FILE:activity-monitor> ${CMAKE_SOURCE_DIR}/bin/activity-monitor.dll)
endif()

# set icon
if(WIN32)
    add_custom_command(TARGET EyeLeo POST_BUILD
        COMMAND ${CMAKE_SOURCE_DIR}/tools/ReplaceVistaIcon.exe
        ${CMAKE_SOURCE_DIR}/bin/EyeLeo.exe ${CMAKE_SOURCE_DIR}/source/resource/icon.ico)
endif()
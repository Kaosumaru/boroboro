cmake_minimum_required (VERSION 2.8)
project (Boroboro)

set(APPLICATION_MAJOR_VERSION 0)
set(APPLICATION_MINOR_VERSION 0)
set(APPLICATION_PATCH_VERSION 1)

set(APPLICATION_NAME "Boroboro")


#snip this later
set(APPLICATION_STRING_VERSION "${APPLICATION_MAJOR_VERSION}.${APPLICATION_MINOR_VERSION}.${APPLICATION_PATCH_VERSION}")


#message(STATUS "TEST ${CMAKE_CURRENT_BINARY_DIR}")
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/ProjectData)
file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/ProjectData/ProjectInfo.txt "${APPLICATION_NAME}\n${APPLICATION_STRING_VERSION}")

#configure build packaging system...
#file(COPY ${PROJECT_SOURCE_DIR}/../Utils/release_make.py DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/ProjectData/)
#file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/ProjectData/deploy.bat "python release_make.py \"${CMAKE_CURRENT_BINARY_DIR}/\" \"${PROJECT_SOURCE_DIR}/\" \npause")
#end



add_definitions(-DMX_APPLICATION_MAJOR_VERSION=${APPLICATION_MAJOR_VERSION})
add_definitions(-DMX_APPLICATION_MINOR_VERSION=${APPLICATION_MINOR_VERSION})
add_definitions(-DMX_APPLICATION_PATCH_VERSION=${APPLICATION_PATCH_VERSION})

add_definitions(-DMX_APPLICATION_NAME="${APPLICATION_NAME}")
#to this.....



option (GAME_PERF_CHEATS "Cheats for perfomance?" FALSE)

if (GAME_PERF_CHEATS)
add_definitions(-DMX_GAME_PERF_CHEATS)
endif(GAME_PERF_CHEATS)

if (GAME_RELEASE)
add_definitions(-DMX_GAME_RELEASE)
endif(GAME_RELEASE)
add_definitions(-DMX_DEBUG_RES_PATH="${PROJECT_SOURCE_DIR}/Res/")

if (MSVC)
add_definitions( "/DMX_PLATFORM_MSVC  /DUNICODE /D_UNICODE" )
add_definitions( "/MP /W3 /D_CRT_SECURE_NO_WARNINGS /wd4005 /wd4996 /wd4250 /wd4244  /wd4503 /nologo /Zm500" )
endif(MSVC)



add_subdirectory(../libmx "${CMAKE_CURRENT_BINARY_DIR}/libmx")
set(CMAKE_MODULE_PATH ${MX_MODULES} CACHE TYPE STRING FORCE)

add_definitions("${MX_DEFINES}")

if (MSVC)
#configure_msvc_runtime()
add_definitions( "/DMX_PLATFORM_MSVC  /DUNICODE /D_UNICODE" )
add_definitions( "/W3 /D_CRT_SECURE_NO_WARNINGS /wd4005 /wd4996 /wd4250 /wd4244 /nologo /Zm180" )
endif(MSVC)

if (EMSCRIPTEN)
  set(CMAKE_EXECUTABLE_SUFFIX ".html")
  add_definitions("-std=c++14")
  set(MX_LINK_FLAGS "${MX_LINK_FLAGS} -s TOTAL_MEMORY=67108864" )
endif(EMSCRIPTEN)

include_directories(${PROJECT_SOURCE_DIR})
include_directories(${MX_INCLUDES})
include(CommonFunctions)

ADDFILES(source_list "${PROJECT_SOURCE_DIR}/game")

add_executable(Boroboro main.cpp  ${source_list})
target_link_libraries(Boroboro ${MX_LIBS} MSL RPN)
set_target_properties(Boroboro PROPERTIES LINK_FLAGS "${MX_LINK_FLAGS}")

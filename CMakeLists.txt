cmake_minimum_required(VERSION 3.0)
project(Iceberg3D)

# Ensure C++14 support
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED on)

# Set path to cmake modules
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake-modules")

# Specify project files
file(GLOB DEPENDENCY_SOURCES Iceberg3D/dependencies/glad/src/glad.c)
file(GLOB PROJECT_HEADERS Iceberg3D/include/*.*)
file(GLOB PROJECT_SOURCES Iceberg3D/source/*.*)
file(GLOB PROJECT_CONFIGS CMakeLists.txt
                          Readme.md
                         .gitattributes
                         .gitignore
                         .gitmodules)

source_group("Headers" FILES ${PROJECT_HEADERS})
source_group("Sources" FILES ${PROJECT_SOURCES})
source_group("Dependencies" FILES ${DEPENDENCY_SOURCES})

add_executable(${PROJECT_NAME} ${PROJECT_HEADERS}
							   ${PROJECT_SOURCES}
                               ${PROJECT_SHADERS}
                               ${PROJECT_CONFIGS}
                               ${DEPENDENCY_SOURCES})
							   
# Include directories
include_directories( Iceberg3D/include/
					 Iceberg3D/dependencies/glad/include/
                     Iceberg3D/dependencies/glm/
                     Iceberg3D/dependencies/stb/)

# Include SDL2, using windows binaries if necessary
message("<Finding SDL2>")
if(WIN32)
	set(SDL2_PATH Iceberg3D/dependencies/sdl)
endif(WIN32)
find_package(SDL2 REQUIRED)
if(SDL2_FOUND)
	include_directories(${SDL2_INCLUDE_DIR})
	target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARY})
else(SDL2_FOUND)
	message(fatal_error "SDL2 COULD NOT BE FOUND!")
endif(SDL2_FOUND)
message("</Finding SDL2>")

# Include FreeType2
message("<Finding FreeType2>")
if(WIN32)
	set(FREETYPE_INCLUDE_DIR_ft2build Iceberg3D/dependencies/freetype2/include)
	set(FREETYPE_INCLUDE_DIR_freetype2 Iceberg3D/dependencies/freetype2/include/freetype2)
	set(FREETYPE_LIBRARY ..\\Iceberg3D\\dependencies\\freetype2\\lib\\freetype)
endif(WIN32)
find_package(Freetype REQUIRED)
if(FREETYPE_FOUND)
	include_directories(${FREETYPE_INCLUDE_DIRS})
	target_link_libraries(${PROJECT_NAME} ${FREETYPE_LIBRARY})
else(FREETYPE_FOUND)
	message(fatal_error "FREETYPE2 COULD NOT BE FOUND!")
endif(FREETYPE_FOUND)
message("</Finding FreeType2>")

# Include Assimp
message("<Finding Assimp>")
if(WIN32)
	set(ASSIMP_INCLUDE_DIR Iceberg3D/dependencies/assimp/include)
	set(ASSIMP_LIBRARY ..\\Iceberg3D\\dependencies\\assimp\\lib\\assimp)
endif(WIN32)
find_package(Assimp REQUIRED)
if(ASSIMP_FOUND)
	include_directories(${ASSIMP_INCLUDE_DIRS})
	target_link_libraries(${PROJECT_NAME} ${ASSIMP_LIBRARY})
else(ASSIMP_FOUND)
	message(fatal_error "ASSIMP COULD NOT BE FOUND!")
endif(ASSIMP_FOUND)
message("</Finding Assimp>")

# Include Bullet physics
# message("<Finding Bullet>")
# if(WIN32)
	# set(BULLET_ROOT Iceberg3D/dependencies/bullet)
# endif(WIN32)
# find_package(Bullet REQUIRED)
# if(BULLET_FOUND)
	# include_directories(${BULLET_INCLUDE_DIRS})
	# target_link_libraries(${PROJECT_NAME} ${BULLET_LIBRARIES})
# else(BULLET_FOUND)
	# message(fatal_error "BULLET COULD NOT BE FOUND!")
# endif(BULLET_FOUND)
# message("</Finding Bullet>")

if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
    # Hide the console in visual studio projects in debug and release
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")
    set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O -w")
    if(NOT WIN32)
        set(GLAD_LIBRARIES dl)
    endif()
endif()

option (ICEBERG_BUILD_SANDBOX "Build testing sandbox" OFF)
if (ICEBERG_BUILD_SANDBOX)
	add_subdirectory(Sandbox/)
endif()

option (ICEBERG_BUILD_DEMOS "Build demo programs" OFF)
if (ICEBERG_BUILD_DEMOS)
	add_subdirectory(Demos/full_scene/)
	add_subdirectory(Demos/basic_triangle/)
	add_subdirectory(Demos/textured_quad/)
endif()

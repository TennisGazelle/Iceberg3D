message("<FindAssimp.cmake>")

find_path(
  ASSIMP_INCLUDE_DIRS
  NAMES postprocess.h scene.h version.h config.h cimport.h
  PATHS /usr/local/include/
)

find_library(
  ASSIMP_LIBRARIES
  NAMES ASSIMP
  PATHS /usr/local/lib/
)

if (ASSIMP_INCLUDE_DIRS AND ASSIMP_LIBRARIES)
  SET(ASSIMP_FOUND TRUE)
endif (ASSIMP_INCLUDE_DIRS AND ASSIMP_LIBRARIES)

if (ASSIMP_FOUND)
	message(STATUS "Found asset importer library: ${ASSIMP_LIBRARIES}")
else (ASSIMP_FOUND)
  if (ASSIMP_FIND_REQUIRED)
	message(FATAL_ERROR "Could not find asset importer library")
  endif (ASSIMP_FIND_REQUIRED)
endif (ASSIMP_FOUND)

message("</FindAssimp.cmake>")
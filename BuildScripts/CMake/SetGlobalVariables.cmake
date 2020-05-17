function(setGlobalVariables)

  # Store the location of Google Test folder
  set(LocationOfGoogleTests
      "${ARGV1}"
      CACHE FORCE "LocationOfGoogleTests")

  # Location where external sources will be downloaded before building.
  set(PATH_FOR_DOWNLOADS
      "${CMAKE_CURRENT_BINARY_DIR}/Downloads"
      CACHE FORCE "PATH_FOR_DOWNLOADS")

  # Location of the BoundedTypes sources required as a dependency to this
  # project.
  set(BOUNDEDTYPES_SRC_DIR
      "${PROJECT_SOURCE_DIR}/../BoundedTypes/CoreFunctions/Library"
      CACHE FORCE "BOUNDEDTYPES_SRC_DIR")

  # Location where the sources for BoundedTypes library will be copied over.
  set(PATH_TO_COPY_BOUNDEDTYPES_SOURCES
      "${CMAKE_CURRENT_BINARY_DIR}/BoundedTypes"
      CACHE FORCE "PATH_TO_COPY_BOUNDEDTYPES_SOURCES")

endfunction(setGlobalVariables)

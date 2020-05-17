include(ExternalProject)

# This function sources the dependency "Bounded Types" used as a library in this
# project.
function(ProvideBoundedTypes)

  set(BoundedTypesWorkingDirectory ${PATH_TO_COPY_BOUNDEDTYPES_SOURCES})

  # set_directory_properties(PROPERTIES EP_BASE
  # ${CMAKE_CURRENT_BINARY_DIR}/ExternalSources)
  ExternalProject_Add(
    BoundedTypes_project
    PREFIX ${BoundedTypesWorkingDirectory} # top level directory in which the
    # repository will be cloned. GIT_REPOSITORY
    # https://github.com/RomanoViolet/FancySquareRoot.git GIT_TAG
    # ${FancySquareRoot_Commit_Hash} UPDATE_DISCONNECTED 1 Download from a local
    # folder instead of cloning from GitHub
    URL ${PROJECT_SOURCE_DIR}/../BoundedTypes/CoreFunctions/Library/
    #
    # UPDATE_COMMAND file (RENAME ${BoundedTypesWorkingDirectory}/Library
    # ${BoundedTypesWorkingDirectory}/BoundedTypes)
    #
    PATCH_COMMAND ""
    BUILD_ALWAYS TRUE
    CONFIGURE_COMMAND ""
    SOURCE_DIR ${BoundedTypesWorkingDirectory}/BoundedTypes/
    INSTALL_DIR ${CMAKE_CURRENT_BINARY_DIR}/LocalInstalls
    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=<INSTALL_DIR> # prefix needs to be
                                                    # specified if INSTALL_DIR
                                                    # is specified.
    INSTALL_COMMAND
      cp
      ${BoundedTypesWorkingDirectory}/src/BoundedTypes_project-build/libBoundedTypes.a
      <INSTALL_DIR>
    #
    CONFIGURE_COMMAND ${CMAKE_COMMAND}
                      ${BoundedTypesWorkingDirectory}/BoundedTypes/
    #
    BUILD_BYPRODUCTS
      ${BoundedTypesWorkingDirectory}/BoundedTypes_project-build/libBoundedTypes.a
  )

  ExternalProject_Get_Property(BoundedTypes_project INSTALL_DIR)
  message("Bounded Types Installed location: ${INSTALL_DIR}")
  add_library(BoundedTypes STATIC IMPORTED)
  set_property(TARGET BoundedTypes PROPERTY IMPORTED_LOCATION
                                            ${INSTALL_DIR}/libBoundedTypes.a)
  add_dependencies(BoundedTypes BoundedTypes_project)

endfunction(ProvideBoundedTypes)

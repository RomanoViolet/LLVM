# Convenience function to be called from the main CMakeLists.txt
function(buildCPPProject)

  # tell CMake where to find all headers required by the sources.

  file(GLOB_RECURSE CPPProject_SOURCES
       ${PROJECT_SOURCE_DIR}/CoreFunctions/Application/*.cpp)
  message("CPPProject Sources: " ${CPPProject_SOURCES})

  # System Headers
  include_directories("/usr/lib/llvm-8/include")

  # Library dependencies: headers
  include_directories(${PATH_TO_COPY_BOUNDEDTYPES_SOURCES})
  include_directories(${PROJECT_SOURCE_DIR}/CoreFunctions)
  link_directories("/usr/lib/llvm-8/lib/")
  add_executable(CPPProject "${CPPProject_SOURCES}")
  target_link_libraries(CPPProject clang)
  set_target_properties(CPPProject PROPERTIES LINKER_LANGUAGE "CXX")

endfunction(buildCPPProject)

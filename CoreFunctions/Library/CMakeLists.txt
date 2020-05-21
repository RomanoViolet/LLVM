include_directories(BEFORE SYSTEM "${PROJECT_SOURCE_DIR}/CoreFunctions"
                    "${PATH_TO_COPY_BOUNDEDTYPES_SOURCES}")

message("Provided path: ${PATH_TO_COPY_BOUNDEDTYPES_SOURCES}")

# Get a list of all source files.
file(GLOB_RECURSE SRC *.hpp *.inl *.cpp)

# Compile the interface files into a library
add_library(Library STATIC ${SRC})
target_link_libraries(Library PUBLIC BoundedTypes)

# In case of .inl files which CMake cannot associate to C++. Set the language
# explicitly. At the moment, only .cpp and .hpp files have been provided.
set_target_properties(Library PROPERTIES LINKER_LANGUAGE CXX)

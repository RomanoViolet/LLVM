# Convenience function to set build standards
macro(setBuildChainSpecifications)

  # Set the received arguments into variables
  set(CompilerChain ${ARGV1})
  # set (LanguageStandard "c++${ARGV3}")
  set(BuildType ${ARGV5})
  set(CMAKE_CXX_STANDARD ${ARGV3})

  # Make the compiler type global so that other functions can also use it.
  string(TOLOWER ${CompilerChain} CompilerChain_alllowerCase)

  # Make build type lowercase to ignore case- related inconsistencies in the
  # user input
  string(TOLOWER ${BuildType} BuildType_alllowerCase)

  # Build type *must* be set outside of CMake. if ("${BuildType_alllowerCase}"
  # STREQUAL "debug") set ( CMAKE_BUILD_TYPE Debug FORCE ) else() set (
  # CMAKE_BUILD_TYPE Release FORCE ) endif()

  # Flags -Wnamespaces was removed in order to compile VFC. -Weffc++ was removed
  # in order to compile VFC. -Wconversion was removed in order to compile VFC.
  # -Werror=sign-promo was removed in order to compile VFC.
  set(CMAKE_CXX_FLAGS_GCC_DEBUG
      "-march=native \
                                    -O0 \
                                    -pg \
                                    -ggdb \
                                    -g3 \
                                    -Wall \
                                    -fdump-rtl-all \
                                    -Werror=reorder \
                                    -fvisibility=hidden \
                                    -Werror=uninitialized \
                                    --no-inline \
                                    -Werror=narrowing \
                                    -Werror=reorder \
                                    -fdiagnostics-show-location=once \
                                    -Werror=unused-parameter \
                                    -fdiagnostics-color=auto \
                                    -Werror=pedantic \
                                    -Warray-bounds \
                                    -fstack-protector-strong \
                                    -fstack-protector \
                                    -D_FORTIFY_SOURCE=2 \
                                    -Werror=unused \
                                    -fprofile-arcs \
                                    -ftest-coverage \
                                    -Wno-unused-parameter")

  set(CMAKE_CXX_FLAGS_CLANG_DEBUG
      "-march=native \
                                    -O0 \
                                    -Xclang \
                                    -disable-O0-optnone \
                                    -pg \
                                    -ggdb \
                                    -g3 \
                                    -Wall \
                                    -Werror=reorder \
                                    -fvisibility=hidden \
                                    -Weffc++ \
                                    -Werror=uninitialized \
                                    -Werror=narrowing \
                                    -Werror=reorder \
                                    -fdiagnostics-show-location=once \
                                    -Werror=unused-parameter \
                                    -fdiagnostics-color=auto \
                                    -Werror=pedantic \
                                    -Warray-bounds \
                                    -fstack-protector-strong \
                                    -fstack-protector \
                                    -D_FORTIFY_SOURCE=2 \
                                    -Wunused-private-field \
                                    -Wignored-attributes \
                                    -Werror=unused-value \
                                    -Wno-c++98-compat \
                                    -Werror=invalid-constexpr \
                                    -Werror=unused-variable")

  set(CMAKE_CXX_FLAGS_GCC_RELEASE
      "-march=native \
                                    -O3 \
                                    -Wall \
                                    -fdump-rtl-all \
                                    -Werror=reorder \
                                    -fvisibility=hidden \
                                    -Werror=uninitialized \
                                    --no-inline \
                                    -Werror=narrowing \
                                    -Werror=reorder \
                                    -fdiagnostics-show-location=once \
                                    -Werror=unused-parameter \
                                    -fdiagnostics-color=auto \
                                    -Werror=pedantic \
                                    -Warray-bounds \
                                    -fstack-protector-strong \
                                    -fstack-protector \
                                    -D_FORTIFY_SOURCE=2 \
                                    -Werror=unused-local-typedefs \
                                    -Werror=unused-value \
                                    -Werror=unused")

  set(CMAKE_CXX_FLAGS_CLANG_RELEASE
      "-march=native \
                                    -O3 \
                                    -Wall \
                                    -Werror=reorder \
                                    -fvisibility=hidden \
                                    -Weffc++ \
                                    -Werror=uninitialized \
                                    -Werror=narrowing \
                                    -Werror=reorder \
                                    -fdiagnostics-show-location=once \
                                    -Werror=unused-parameter \
                                    -fdiagnostics-color=auto \
                                    -Werror=pedantic \
                                    -Warray-bounds \
                                    -fstack-protector-strong \
                                    -fstack-protector \
                                    -D_FORTIFY_SOURCE=2 \
                                    -Wunused-private-field \
                                    -Werror=unused-value \
                                    -Wno-c++98-compat \
                                    -Werror=invalid-constexpr \
                                    -Werror=unused-variable")

  if("${CompilerChain_alllowerCase}" STREQUAL "gcc")
    set(CMAKE_CXX_FLAGS_RELEASE
        "${CMAKE_CXX_FLAGS_RELEASE} ${CMAKE_CXX_FLAGS_GCC_RELEASE}"
        CACHE STRING "" FORCE)
    set(CMAKE_CXX_FLAGS_DEBUG
        "${CMAKE_CXX_FLAGS_DEBUG} ${CMAKE_CXX_FLAGS_GCC_DEBUG}"
        CACHE STRING "" FORCE)
    # set(CMAKE_CXX_COMPILER g++)
    set(CXX_STANDARD_REQUIRED true)
    set(CMAKE_CXX_EXTENSIONS off)

  elseif("${CompilerChain_alllowerCase}" STREQUAL "llvm"
         OR "${CompilerChain_alllowerCase}" STREQUAL "clang")
    set(CMAKE_CXX_FLAGS_RELEASE
        "${CMAKE_CXX_FLAGS_RELEASE} ${CMAKE_CXX_FLAGS_CLANG_RELEASE}"
        CACHE STRING "" FORCE)
    set(CMAKE_CXX_FLAGS_DEBUG
        "${CMAKE_CXX_FLAGS_DEBUG} ${CMAKE_CXX_FLAGS_CLANG_DEBUG}"
        CACHE STRING "" FORCE)
    # set(CMAKE_CXX_COMPILER clang++)
    set(CXX_STANDARD_REQUIRED true)
    set(CMAKE_CXX_EXTENSIONS off)

  elseif("${CompilerChain_alllowerCase}" STREQUAL "mingw")
    set(CMAKE_CXX_FLAGS_RELEASE
        "${CMAKE_CXX_FLAGS_RELEASE} ${CMAKE_CXX_FLAGS_GCC_RELEASE}"
        CACHE STRING "" FORCE)
    set(CMAKE_CXX_FLAGS_DEBUG
        "${CMAKE_CXX_FLAGS_DEBUG} ${CMAKE_CXX_FLAGS_GCC_DEBUG}"
        CACHE STRING "" FORCE)
    # set(CMAKE_CXX_COMPILER g++)
    set(CXX_STANDARD_REQUIRED true)
    set(CMAKE_CXX_EXTENSIONS off)

  elseif("${CompilerChain_alllowerCase}" STREQUAL "msvc")
    set(CXX_STANDARD_REQUIRED true)
    set(CMAKE_CXX_EXTENSIONS off)
    # set(CMAKE_CXX_COMPILER msvc)
    set(CMAKE_C_FLAGS_DEBUG "/MTd /Z7 /Od")
    set(CMAKE_CXX_FLAGS_DEBUG "/MTd /Zi")
    set(gtest_force_shared_crt CACHE FORCE TRUE)
    set(gmock_force_shared_crt CACHE FORCE TRUE)
    set(CMAKE_CXX_FLAGS_RELEASE "/MT")

  elseif("${CompilerChain_alllowerCase}" STREQUAL "ghs")
    include(Compiler/GHS)
    set(CMAKE_CXX_VERBOSE_FLAG "-v")
    set(CMAKE_CXX_OUTPUT_EXTENSION ".o")

    string(APPEND CMAKE_CXX_FLAGS_INIT " ")
    string(APPEND CMAKE_CXX_FLAGS_DEBUG_INIT " -Odebug -g")
    string(APPEND CMAKE_CXX_FLAGS_MINSIZEREL_INIT " -Ospace")
    string(APPEND CMAKE_CXX_FLAGS_RELEASE_INIT " -O")
    string(APPEND CMAKE_CXX_FLAGS_RELWITHDEBINFO_INIT " -O -g")
    set(CXX_STANDARD_REQUIRED true)
    set(CMAKE_CXX_EXTENSIONS off)

  endif()

endmacro(setBuildChainSpecifications)

# https://foonathan.net/blog/2016/03/03/cmake-install.html Setting build chain
# preferences
include(
  ${CMAKE_CURRENT_SOURCE_DIR}/BuildScripts/CMake/SetBuildChainSpecifications.cmake
)

cmake_minimum_required(VERSION 3.10)

# Supply arguments as key-value pairs CompilerChain: gcc | llvm | msvc | mingw
# LanguageStandard: 11 | 14 | 17 BuildType: Debug | Release
setbuildchainspecifications(CompilerChain ${COMPILER} LanguageStandard
                            ${STANDARD} BuildType ${CMAKE_BUILD_TYPE})

# The name of the project
project(
  CPPApplication
  VERSION 0.3
  LANGUAGES CXX)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Tool Compliance
include(${CMAKE_CURRENT_SOURCE_DIR}/BuildScripts/CMake/ToolCompliance.cmake)

# For setting global variables, such as required directories
include(${CMAKE_CURRENT_SOURCE_DIR}/BuildScripts/CMake/SetGlobalVariables.cmake)

# Function to build DemoApplication as a library. Not everything is linked yet.
include(${CMAKE_CURRENT_SOURCE_DIR}/BuildScripts/CMake/BuildDemoLibrary.cmake)

# Convenience functions for installing DemoApplication locally
include(
  ${CMAKE_CURRENT_SOURCE_DIR}/BuildScripts/CMake/InstallDemoApplicationLocally.cmake
)

# All DemoApplication Internal tests, e.g., Google Tests
include(
  ${CMAKE_CURRENT_SOURCE_DIR}/BuildScripts/CMake/BuildAndRunAllGoogleTests.cmake
)

# Ensure that all tools and versions are compliant. Minimum acceptable versions
# are speciied as key-value pairs Currently, only minimum git version is
# required
ensuretoolcompliance(Git 2.17)

# Set Global variables such as the commit-hash of the FancySquareRoot library
# that must be used.
setglobalvariables(Location_of_GoogleTests "${PROJECT_SOURCE_DIR}/GoogleTests")

# Build CPPProject as a library, but is not yet linked to external dependencies,
# as FancySquareRoot library
buildcppproject()

# Install the built package locally in order to support find_package
# installcppproject( Location "${PROJECT_SOURCE_DIR}/build/DemoLibrary"
# VERSION_MAJOR 0 # The version of the library is set to
# VERSION_MAJOR.VERSION_MINOR VERSION_MINOR 9)

# Debug step

# add_custom_target( DummyTarget DEPENDS CopyLegacyDependency COMMENT "Check
# whether the custom square root download and build worked" )
# ------------------Testing-----------------
# Build and Run all Google Tests
buildandrunallgoogletests()

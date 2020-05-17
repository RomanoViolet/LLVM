# Do not assume that the Google Test framework is preinstalled. Instead, clone
# the library, and buiid it locally. Adapted from :
# https://gist.github.com/johnb003/65982fdc7a1274fdb023b0c68664ebe4 Paths for
# libraries are updated.
include(ExternalProject)

function(ProvideGoogleTest)
  find_package(GTest QUIET)
  if(NOT GTEST_FOUND)
    message(
      "--------------------------------------------------------------------------------"
    )
    message(
      "                                                                                "
    )
    message(
      " Google Tests will be downloaded from Google's Git Repository, and will be built. "
    )
    message(
      "                                                                                "
    )
    message(
      "--------------------------------------------------------------------------------"
    )
    buildgoogletestsfromsource()
  else()
    message(
      "--------------------------------------------------------------------------------"
    )
    message(
      "                                                                                "
    )
    message(" Found Google Tests installed on the system. ")
    message(
      "                                                                                "
    )
    message(
      "--------------------------------------------------------------------------------"
    )
    enable_testing()
    find_package(GTest REQUIRED)
    add_custom_target(AllGoogleTests ALL DEPENDS ${UNIT_TEST_TARGETS})
    add_subdirectory("${LocationOfGoogleTests}")
  endif(NOT GTEST_FOUND)
endfunction(ProvideGoogleTest)

function(BuildGoogleTestsFromSource)
  find_package(Threads REQUIRED)
  ExternalProject_Add(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    UPDATE_COMMAND ""
    INSTALL_COMMAND ""
    LOG_DOWNLOAD ON
    LOG_CONFIGURE ON
    LOG_BUILD ON)

  ExternalProject_Get_Property(googletest source_dir)
  set(GTEST_INCLUDE_DIRS ${source_dir}/googletest/include)
  set(GMOCK_INCLUDE_DIRS ${source_dir}/googlemock/include)

  # The cloning of the above repo doesn't happen until make, however if the dir
  # doesn't exist, INTERFACE_INCLUDE_DIRECTORIES will throw an error. To make it
  # work, we just create the directory now during config.
  file(MAKE_DIRECTORY ${GTEST_INCLUDE_DIRS})
  file(MAKE_DIRECTORY ${GMOCK_INCLUDE_DIRS})

  ExternalProject_Get_Property(googletest binary_dir)

  set(GTEST_LIBRARY_PATH
      ${binary_dir}/lib/${CMAKE_FIND_LIBRARY_PREFIXES}gtest.a)
  set(GTEST_LIBRARY gtest)
  add_library(${GTEST_LIBRARY} UNKNOWN IMPORTED)
  set_target_properties(
    ${GTEST_LIBRARY}
    PROPERTIES "IMPORTED_LOCATION" "${GTEST_LIBRARY_PATH}"
               "IMPORTED_LINK_INTERFACE_LIBRARIES" "${CMAKE_THREAD_LIBS_INIT}"
               "INTERFACE_INCLUDE_DIRECTORIES" "${GTEST_INCLUDE_DIRS}")
  add_dependencies(${GTEST_LIBRARY} googletest)

  set(GTEST_MAIN_LIBRARY_PATH
      ${binary_dir}/lib/${CMAKE_FIND_LIBRARY_PREFIXES}gtest_main.a
      CACHE FORCE "GTEST_MAIN_LIBRARY_PATH")

  set(GTEST_MAIN_LIBRARY
      gtest_main
      CACHE FORCE "GTEST_MAIN_LIBRARY")

  add_library(${GTEST_MAIN_LIBRARY} UNKNOWN IMPORTED)
  set_target_properties(
    ${GTEST_MAIN_LIBRARY}
    PROPERTIES "IMPORTED_LOCATION" "${GTEST_MAIN_LIBRARY_PATH}"
               "IMPORTED_LINK_INTERFACE_LIBRARIES" "${CMAKE_THREAD_LIBS_INIT}"
               "INTERFACE_INCLUDE_DIRECTORIES" "${GTEST_INCLUDE_DIRS}")
  add_dependencies(${GTEST_MAIN_LIBRARY} googletest)

  set(GMOCK_LIBRARY_PATH
      ${binary_dir}/lib/${CMAKE_FIND_LIBRARY_PREFIXES}gmock.a
      CACHE FORCE "GMOCK_LIBRARY_PATH")

  set(GMOCK_LIBRARY
      gmock
      CACHE FORCE "GMOCK_LIBRARY")
  add_library(${GMOCK_LIBRARY} UNKNOWN IMPORTED)
  set_target_properties(
    ${GMOCK_LIBRARY}
    PROPERTIES "IMPORTED_LOCATION" "${GMOCK_LIBRARY_PATH}"
               "IMPORTED_LINK_INTERFACE_LIBRARIES" "${CMAKE_THREAD_LIBS_INIT}"
               "INTERFACE_INCLUDE_DIRECTORIES" "${GMOCK_INCLUDE_DIRS}")
  add_dependencies(${GMOCK_LIBRARY} googletest)

  set(GMOCK_MAIN_LIBRARY_PATH
      ${binary_dir}/lib/${CMAKE_FIND_LIBRARY_PREFIXES}gmock_main.a
      CACHE FORCE "GMOCK_MAIN_LIBRARY_PATH")

  set(GMOCK_MAIN_LIBRARY
      gmock_main
      CACHE FORCE "GMOCK_MAIN_LIBRARY")

  add_library(${GMOCK_MAIN_LIBRARY} UNKNOWN IMPORTED)
  set_target_properties(
    ${GMOCK_MAIN_LIBRARY}
    PROPERTIES "IMPORTED_LOCATION" "${GMOCK_MAIN_LIBRARY_PATH}"
               "IMPORTED_LINK_INTERFACE_LIBRARIES" "${CMAKE_THREAD_LIBS_INIT}"
               "INTERFACE_INCLUDE_DIRECTORIES" "${GMOCK_INCLUDE_DIRS}")
  add_dependencies(${GMOCK_MAIN_LIBRARY} ${GTEST_LIBRARY})

  add_custom_target(AllGoogleTests ALL DEPENDS ${UNIT_TEST_TARGETS})

  add_subdirectory("${LocationOfGoogleTests}")

endfunction(BuildGoogleTestsFromSource)

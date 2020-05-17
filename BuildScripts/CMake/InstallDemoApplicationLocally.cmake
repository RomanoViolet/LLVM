# https://pabloariasal.github.io/2018/02/19/its-time-to-do-cmake-right/
# https://rix0r.nl/blog/2015/08/13/cmake-guide/
# https://www.slideshare.net/DanielPfeifer1/cmake-48475415
# https://cliutils.gitlab.io/modern-cmake/chapters/install/installing.html
function(InstallCPPProject)

  # The following custom command will cause the install to always execute after
  # the build. Else, "make install" needs to be called separately.
  # https://stackoverflow.com/a/29712310
  add_custom_command(
    TARGET CPPProject POST_BUILD COMMAND ${CMAKE_COMMAND} --build
                                         ${CMAKE_BINARY_DIR} --target install)

  # Suppress file-by-file install step notifications
  set(CMAKE_INSTALL_MESSAGE "NEVER")

  # Get the user supplied install location.
  set(InstallLocation "${ARGV1}")

  # Set the version of the library Adapted from
  # https://foonathan.net/blog/2016/03/03/cmake-install.html
  set(CPPProject_VERSION_MAJOR
      "${ARGV3}"
      CACHE STRING "Major Version" FORCE)

  set(CPPProject_VERSION_MINOR
      "${ARGV5}"
      CACHE STRING "Minor Version" FORCE)

  set(CPPProject_VERSION
      ${CPPProject_VERSION_MAJOR}.${CPPProject_VERSION_MINOR}
      CACHE STRING "version" FORCE)

  # paths to include/ and library
  set(CPPProject_IncPath "${InstallLocation}-${CPPProject_VERSION}/inc")
  set(CPPProject_LibPath "${InstallLocation}-${CPPProject_VERSION}/lib")

  file(MAKE_DIRECTORY "${CPPProject_IncPath}")
  file(MAKE_DIRECTORY "${CPPProject_LibPath}")

  # Install the CPPProject Install does not like targets created from another
  # file: https://stackoverflow.com/questions/34443128/cmake-install-targets-in-
  # subdirectories
  install(
    TARGETS CPPProject
    EXPORT CPPProject-export
    LIBRARY DESTINATION "${CPPProject_LibPath}"
    ARCHIVE DESTINATION "${CPPProject_LibPath}")

  install(
    EXPORT CPPProject-export
    FILE CPPProjectTargets.cmake
    NAMESPACE DL::
    DESTINATION "${CPPProject_LibPath}")

  # Make the header files available Collect the list of header files to be
  # exported
  file(GLOB_RECURSE CPPProjectHeaders
       ${PROJECT_SOURCE_DIR}/CoreFunctions/Interfaces/*.hpp)
  install(FILES ${CPPProjectHeaders} DESTINATION "${CPPProject_IncPath}")

endfunction(InstallCPPProject)

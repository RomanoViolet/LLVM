function(ensureToolCompliance)

  # Optimization: Add a loop rather than manually unrolling the key-value pairs

  # get the version number into a local variable
  set(GitVersion "${ARGV1}")
  
  # and set the requirement. If the package is found, ${GIT_EXECUTABLE} is set
  # globally.
  find_package(Git "${ARGV1}" REQUIRED)

endfunction(ensureToolCompliance)

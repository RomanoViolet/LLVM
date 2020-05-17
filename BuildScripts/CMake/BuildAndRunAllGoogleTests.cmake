include(${CMAKE_CURRENT_SOURCE_DIR}/BuildScripts/CMake/RunGoogleTests.cmake)
include(${CMAKE_CURRENT_SOURCE_DIR}/BuildScripts/CMake/BuildGoogleTests.cmake)
include(CTest)
function(BuildAndRunAllGoogleTests)
  
  ProvideGoogleTest()
  Rungoogletests()

endfunction(BuildAndRunAllGoogleTests)

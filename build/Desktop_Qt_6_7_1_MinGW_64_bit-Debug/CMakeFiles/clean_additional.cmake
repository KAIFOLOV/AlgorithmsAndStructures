# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "AlgorithmsAndStructures_autogen"
  "CMakeFiles\\AlgorithmsAndStructures_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\AlgorithmsAndStructures_autogen.dir\\ParseCache.txt"
  )
endif()

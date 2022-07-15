cmake_minimum_required(VERSION 3.0.0)
project(IDSiMCache VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)

include(CTest)
enable_testing()

include_directories(./dataStructure/include)
include_directories(./cache/include)
include_directories(./traceloader/include)
include_directories(./profiler/include)
include_directories(./utils)
include_directories(./analyzer/include)

aux_source_directory(./dataStructure dataStructure)
aux_source_directory(./cache cache)
aux_source_directory(./traceloader traceloader)
aux_source_directory(./profiler profiler)
aux_source_directory(./analyzer analyzer)


add_executable(testLFU ./test/testLFU.cc ${dataStructure} ${cache} ${traceloader} ${profiler})
add_executable(testLRU ./test/testLRU.cc ${dataStructure} ${cache} ${traceloader} ${profiler})
add_executable(testMRC ./test/testMRC.cc ${dataStructure} ${cache} ${traceloader} ${profiler})
add_executable(testNormalTrace ./test/testNormalTrace.cc ${dataStructure} ${cache} ${traceloader} ${profiler})
add_executable(testBasicInfo ./test/testBasicInfo.cc ${dataStructure} ${cache} ${traceloader} ${profiler} ${analyzer})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
SET (EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
include(CPack)

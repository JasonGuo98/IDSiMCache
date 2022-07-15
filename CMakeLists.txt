cmake_minimum_required(VERSION 3.0.0)
project(IDSiMCache VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)

include(CTest)
enable_testing()

include_directories(./datastructure/include)
include_directories(./cache/include)
include_directories(./traceloader/include)
include_directories(./profiler/include)
include_directories(./utils)
include_directories(./analyzer/include)


aux_source_directory(./datastructure datastructure)
aux_source_directory(./cache cache)
aux_source_directory(./traceloader traceloader)
aux_source_directory(./profiler profiler)
aux_source_directory(./analyzer analyzer)


add_executable(testLFU ./test/testLFU.cc ${datastructure} ${cache} ${traceloader} ${profiler})#Windows生成.exe文件 Linux生成无扩展名文件
add_library(TestLFU SHARED ./test/testLFU.cc ${datastructure} ${cache} ${traceloader} ${profiler})#Windows生成.dll和.dll.a文件 Linux生成.so文件

add_executable(testLRU ./test/testLRU.cc ${datastructure} ${cache} ${traceloader} ${profiler})
add_library(TestLRU SHARED ./test/testLRU.cc ${datastructure} ${cache} ${traceloader} ${profiler})

add_executable(testMRC ./test/testMRC.cc ${datastructure} ${cache} ${traceloader} ${profiler})
add_library(TestMRC SHARED ./test/testMRC.cc ${datastructure} ${cache} ${traceloader} ${profiler})

add_executable(testNormalTrace ./test/testNormalTrace.cc ${datastructure} ${cache} ${traceloader} ${profiler})
add_library(TestNormalTrace SHARED ./test/testNormalTrace.cc ${datastructure} ${cache} ${traceloader} ${profiler})

add_executable(testBasicInfo ./test/testBasicInfo.cc ${datastructure} ${cache} ${traceloader} ${profiler} ${analyzer})
add_library(TestBasicInfo SHARED ./test/testBasicInfo.cc ${datastructure} ${cache} ${traceloader} ${profiler} ${analyzer})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
SET (EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)#可执行文件生成在bin文件夹下
SET (LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)#库文件放生成在lib文件夹下
include(CPack)

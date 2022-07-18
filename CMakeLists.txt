cmake_minimum_required(VERSION 3.0.0)
project(IDSiMCache VERSION 0.1.0)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})

set(CMAKE_CXX_STANDARD 17)

include(CPack)

include(CTest)
enable_testing()

set(CMAKE_SHARED_LIBRARY_PREFIX "")#去除生成的库文件的lib前缀
set(CMAKE_STATIC_LIBRARY_PREFIX "")#去除生成的库文件的lib前缀

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
# add_library(_testLFU SHARED ./test/testLFU.cc ${datastructure} ${cache} ${traceloader} ${profiler})#Windows生成.dll和.dll.a文件 Linux生成.so文件

add_executable(testLRU ./test/testLRU.cc ${datastructure} ${cache} ${traceloader} ${profiler})
# add_library(_testLRU SHARED ./test/testLRU.cc ${datastructure} ${cache} ${traceloader} ${profiler})

add_executable(testARC ./test/testARC.cc ${datastructure} ${cache} ${traceloader} ${profiler})
# add_library(_testARC SHARED ./test/testARC.cc ${datastructure} ${cache} ${traceloader} ${profiler})

add_executable(testMRC ./test/testMRC.cc ${datastructure} ${cache} ${traceloader} ${profiler})
# add_library(_testMRC SHARED ./test/testMRC.cc ${datastructure} ${cache} ${traceloader} ${profiler})

add_executable(testNormalTrace ./test/testNormalTrace.cc ${datastructure} ${cache} ${traceloader} ${profiler})
# add_library(_testNormalTrace SHARED ./test/testNormalTrace.cc ${datastructure} ${cache} ${traceloader} ${profiler})

add_executable(testBasicInfo ./test/testBasicInfo.cc ${datastructure} ${cache} ${traceloader} ${profiler} ${analyzer})
# add_library(_testBasicInfo SHARED ./test/testBasicInfo.cc ${datastructure} ${cache} ${traceloader} ${profiler} ${analyzer})

SET (EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)#可执行文件生成在bin文件夹下
SET (LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)#库文件放生成在lib文件夹下

#------------------python---------------------

# 如果使用的是非系统目录下的 Python 可以通过指定 Python3_ROOT_DIR 改变查找路径
IF (CMAKE_SYSTEM_NAME MATCHES "Linux")
    MESSAGE(STATUS "current platform: Linux ")
    # list(APPEND CMAKE_PREFIX_PATH "/usr/include/python3.8")
    # set(CMAKE_FIND_FRAMEWORK NEVER)
    # set(CMAKE_FIND_USE_CMAKE_PATH FALSE)
    set(Python3_FIND_STRATEGY LOCATION)
    set(Python3_ROOT_DIR "/home/wzj/anaconda3")#填python环境的路径
    # set(Python3_ROOT_DIR "/home/wzj/anaconda3/envs/temp")    

    #使用find_package(python COMPONENTS numpy)总是报错，显示找不到numpy，可能是电脑相关配置的原因
    #这里直接将要用到的numpy文件通过路径添加进来
    set(Python3_NumPy_INCLUDE_DIRS "/home/wzj/anaconda3/lib/site-packages/numpy/core/include")
    include_directories(${Python3_NumPy_INCLUDE_DIRS})

ELSEIF (CMAKE_SYSTEM_NAME MATCHES "Windows")
    MESSAGE(STATUS "current platform: Windows")
    # set(Python3_FIND_STRATEGY LOCATION)
    # set(Python3_ROOT_DIR "D:/Anaconda/envs/myenvs")#填python环境的路径

    #使用find_package(python COMPONENTS numpy)总是报错，显示找不到numpy，可能是电脑相关配置的原因
    #这里直接将要用到的numpy文件通过路径添加进来
    set(Python3_NumPy_INCLUDE_DIRS "D:/Anaconda/Lib/site-packages/numpy/core/include")
    include_directories(${Python3_NumPy_INCLUDE_DIRS})

ENDIF (CMAKE_SYSTEM_NAME MATCHES "Linux")


find_package(Python3 COMPONENTS Interpreter Development REQUIRED)
# find_package(Python3 COMPONENTS numpy REQUIRED)#加了numpy之后就会报错：miss numpy

#输出一些cmake自动找到的系统python环境参数
message(STATUS "<<++=====------------------\\/------------------=====++>>")
message(STATUS "<<++              IDSiMCache summary             ++>>")
message(STATUS "<<++=====------------------/\\------------------=====++>>")
MESSAGE(STATUS "Python3_ROOT_DIR:" ${Python3_ROOT_DIR})
MESSAGE(STATUS "Python3_INCLUDE_DIRS:" ${Python3_INCLUDE_DIRS})
MESSAGE(STATUS "Python3_LIBRARIES:" ${Python3_LIBRARIES})
MESSAGE(STATUS "Python3_LIBRARY_DIRS:" ${Python3_LIBRARY_DIRS})
MESSAGE(STATUS "Python3_EXECUTABLE:" ${Python3_EXECUTABLE})
MESSAGE(STATUS "Python3_VERSION:" ${Python3_VERSION})
MESSAGE(STATUS "Python3_NumPy_INCLUDE_DIRS:" ${Python3_NumPy_INCLUDE_DIRS})
MESSAGE(STATUS "Python3_NumPy_VERSION:" ${Python3_NumPy_VERSION})
MESSAGE(STATUS "Python3_NumPy_FOUND:" ${Python3_NumPy_FOUND})
MESSAGE(STATUS "Python3_FIND_VIRTUALENV:" ${Python3_FIND_VIRTUALENV})
MESSAGE(STATUS "CMAKE_MODULE_PATH: " ${CMAKE_MODULE_PATH})


include_directories(./pywrapper/include)
aux_source_directory(./pywrapper pywrapper)

include_directories(${Python3_INCLUDE_DIRS})
add_library(c_MrcSim SHARED ./pywrapper/py_mrc_sim_wrapper.cc ${datastructure} ${cache} ${traceloader} ${profiler} ${analyzer} ${pywrapper})
target_include_directories(c_MrcSim PRIVATE ${Python3_INCLUDE_DIRS} ${Python3_NumPy_INCLUDE_DIRS})
target_link_libraries(c_MrcSim ${Python3_LIBRARIES})




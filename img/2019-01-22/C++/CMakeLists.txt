PROJECT(boost_task_operaction)
CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
MESSAGE(STATUS "#########################")

SET(CMAKE_BUILD_TYPE Release)
ADD_DEFINITIONS(-std=c++11 -g -Wall )
SET(CMAKE_VERBOSE_MAKEFILE ON)
file(GLOB ALL_CXX_SRCS
./*.cpp
)

add_executable(${PROJECT_NAME} ${ALL_CXX_SRCS} )

set(ALL_LIB  pthread
	)


target_link_libraries(${PROJECT_NAME} ${ALL_LIB})



cmake_minimum_required(VERSION 2.8.3)
project(coverage)

find_package(catkin REQUIRED COMPONENTS
  message_generation
  roscpp
  std_msgs
)

add_message_files(
	DIRECTORY
	msg
	FILES
	task_info.msg
	coverage_info.msg
)

generate_messages(
  DEPENDENCIES
  std_msgs
)

catkin_package(
#  INCLUDE_DIRS include
#  LIBRARIES coverage
#  CATKIN_DEPENDS message_generation roscpp std_msgs
#  DEPENDS system_lib
	CATKIN_DEPENDS message_runtime
)

include_directories(
# include
  ${catkin_INCLUDE_DIRS}
)

add_executable(coverage_node src/coverage_node.cpp)
add_dependencies(coverage_node coverage_generate_messages_cpp)
target_link_libraries(coverage_node ${catkin_LIBRARIES})

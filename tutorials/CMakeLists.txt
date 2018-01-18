cmake_minimum_required(VERSION 2.8.3)
project(tutorials)

find_package(catkin REQUIRED COMPONENTS
  roscpp
  std_msgs
  message_generation ## add!!
)

add_message_files(
  DIRECTORY
  msg
  FILES
  Num.msg
)
generate_messages(
  DEPENDENCIES
  std_msgs
)

catkin_package(
#  INCLUDE_DIRS include
#  LIBRARIES tutorials
#  CATKIN_DEPENDS roscpp std_msgs
#  DEPENDS system_lib
CATKIN_DEPENDS message_runtime ## add!!
)

include_directories(
# include
  ${catkin_INCLUDE_DIRS}
)

#add_executable(talker src/talker.cpp)
#target_link_libraries(talker ${catkin_LIBRARIES})
#add_dependencies(talker tutorials_generate_messages_cpp)

add_executable(listener src/listener.cpp)
target_link_libraries(listener ${catkin_LIBRARIES})
add_dependencies(listener tutorials_generate_messages_cpp)


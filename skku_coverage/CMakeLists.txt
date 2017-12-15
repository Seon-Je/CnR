cmake_minimum_required(VERSION 2.8.3)
project(skku_coverage)

find_package(catkin REQUIRED COMPONENTS
  message_generation
  roscpp
  std_msgs
)

add_message_files(FILES skku_coverage_msg.msg)

generate_messages(DEPENDENCIES std_msgs)

catkin_package(
##  INCLUDE_DIRS include
  LIBRARIES skku_coverage
  CATKIN_DEPENDS roscpp std_msgs
##  DEPENDS system_lib
)

include_directories(
  ${catkin_INCLUDE_DIRS}
)

add_executable(${PROJECT_NAME}_node src/skku_coverage_node.cpp)

add_dependencies(${PROJECT_NAME}_node ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})

target_link_libraries(${PROJECT_NAME}_node
   ${catkin_LIBRARIES}
 )

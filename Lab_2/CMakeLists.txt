cmake_minimum_required( VERSION 2.8 )
project( csc232 )
set( CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR} CACHE PATH "default install prefix" FORCE )
set( ROS_PATH_PREFIX "/opt/ros/indigo" )

find_program(GENGETOPT gengetopt)

message("GENGETOPT:${GENGETOPT}")
message("CMAKE_CURRENT_SOURCE_DIR:${CMAKE_CURRENT_SOURCE_DIR}")
message("CMAKE_CURRENT_BINARY_DIR:${CMAKE_CURRENT_BINARY_DIR}")


execute_process( COMMAND ${GENGETOPT} -i ${CMAKE_CURRENT_SOURCE_DIR}/sim_odom_pub.ggo --file-name sim_odom_pub_cmdline --output-dir=${CMAKE_CURRENT_BINARY_DIR} --unamed-opts )
execute_process( COMMAND ${GENGETOPT} -i ${CMAKE_CURRENT_SOURCE_DIR}/sim_empty_sub.ggo --file-name sim_empty_sub_cmdline --output-dir=${CMAKE_CURRENT_BINARY_DIR} --unamed-opts )
execute_process( COMMAND ${GENGETOPT} -i ${CMAKE_CURRENT_SOURCE_DIR}/sim_twist_sub.ggo --file-name sim_twist_sub_cmdline --output-dir=${CMAKE_CURRENT_BINARY_DIR} --unamed-opts )

execute_process( COMMAND ${GENGETOPT} -i ${CMAKE_CURRENT_SOURCE_DIR}/nav_empty_pub.ggo --file-name nav_empty_pub_cmdline --output-dir=${CMAKE_CURRENT_BINARY_DIR} --unamed-opts )
execute_process( COMMAND ${GENGETOPT} -i ${CMAKE_CURRENT_SOURCE_DIR}/nav_navi_pub.ggo --file-name nav_navi_pub_cmdline --output-dir=${CMAKE_CURRENT_BINARY_DIR} --unamed-opts )
execute_process( COMMAND ${GENGETOPT} -i ${CMAKE_CURRENT_SOURCE_DIR}/nav_odom_sub.ggo --file-name nav_odom_sub_cmdline --output-dir=${CMAKE_CURRENT_BINARY_DIR} --unamed-opts )



find_path(ROS_INCLUDE_DIR NAMES ros/ros.h HINTS CMAKE_INSTALL_PREFIX/include)
include_directories(${ROS_INCLUDE_DIR} ${PROJECT_BINARY_DIR})
file( GLOB ROS_LIBRARIES "${ROS_PATH_PREFIX}/lib/*.so*" )



add_executable( sim_odom_pub sim_odom_pub.cpp ${CMAKE_CURRENT_BINARY_DIR}/sim_odom_pub_cmdline.c )
add_executable( sim_empty_sub sim_empty_sub.cpp ${CMAKE_CURRENT_BINARY_DIR}/sim_empty_sub_cmdline.c )
add_executable( sim_twist_sub sim_twist_sub.cpp ${CMAKE_CURRENT_BINARY_DIR}/sim_twist_sub_cmdline.c )

add_executable( nav_empty_pub nav_empty_pub.cpp ${CMAKE_CURRENT_BINARY_DIR}/nav_empty_pub_cmdline.c )
add_executable( nav_navi_pub nav_navi_pub.cpp ${CMAKE_CURRENT_BINARY_DIR}/nav_navi_pub_cmdline.c )
add_executable( nav_odom_sub nav_odom_sub.cpp ${CMAKE_CURRENT_BINARY_DIR}/nav_odom_sub_cmdline.c )



target_link_libraries( sim_odom_pub ${ROS_LIBRARIES} )
target_link_libraries( sim_empty_sub ${ROS_LIBRARIES} )
target_link_libraries( sim_twist_sub ${ROS_LIBRARIES} )

target_link_libraries( nav_empty_pub ${ROS_LIBRARIES} )
target_link_libraries( nav_navi_pub ${ROS_LIBRARIES} )
target_link_libraries( nav_odom_sub ${ROS_LIBRARIES} )


install( TARGETS sim_odom_pub DESTINATION bin)
install( TARGETS sim_empty_sub DESTINATION bin)
install( TARGETS sim_twist_sub DESTINATION bin)

install( TARGETS nav_empty_pub DESTINATION bin)
install( TARGETS nav_navi_pub DESTINATION bin)
install( TARGETS nav_odom_sub DESTINATION bin)












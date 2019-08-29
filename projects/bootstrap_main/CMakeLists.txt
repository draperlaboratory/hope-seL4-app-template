cmake_minimum_required(VERSION 3.7.2)

project(bootstrap_main C)

# target.o must contain isp_main()
add_executable(bootstrap_main src/bootstrap.c)
target_link_libraries(bootstrap_main ${CMAKE_SOURCE_DIR}/projects/bootstrap_main/src/target.a)

target_link_libraries(bootstrap_main sel4 muslc utils platsupport sel4muslcsys sel4platsupport sel4utils sel4debug sel4allocman)

# Set this image as the rootserver
DeclareRootserver(bootstrap_main)

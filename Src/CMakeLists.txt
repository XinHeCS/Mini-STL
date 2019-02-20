aux_source_directory(. SRC_ALL)

add_subdirectory(./Test/Container)
add_subdirectory(./Test/Algorithm)

add_executable(Demo ${SRC_ALL})
target_link_libraries(Demo ContainerModel AlgorithmModel)
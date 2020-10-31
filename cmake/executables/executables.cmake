include_directories(${INCLUDE_DIR})

file (GLOB HEADERS "${INCLUDE_DIR}/*.h" "${INCLUDE_DIR}/*.hpp")

add_executable(rclst
        "${SRC_DIR}/clusterizator.cpp"
        "${SRC_DIR}/version.cpp"
        ${HEADERS})
set_target_properties(rclst PROPERTIES ${TARGET_PROPERTIES})
target_link_libraries(rclst dlib openblas)
target_compile_options(rclst PRIVATE ${COMPILE_OPTIONS})

add_executable(rclss
        "${SRC_DIR}/classificator.cpp"
        "${SRC_DIR}/version.cpp"
        ${HEADERS})
set_target_properties(rclss PROPERTIES ${TARGET_PROPERTIES})
target_link_libraries(rclss dlib openblas)
target_compile_options(rclss PRIVATE ${COMPILE_OPTIONS})

install(TARGETS rclst RUNTIME DESTINATION bin)
install(TARGETS rclss RUNTIME DESTINATION bin)


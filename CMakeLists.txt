cmake_minimum_required(VERSION 2.8)

set(PROJECT_NAME wineggx)
project(${PROJECT_NAME})

include_directories(${PROJECT_SOURCE_DIR}/include)

add_library(${PROJECT_NAME} STATIC src/wineggx.cpp)

set_property(TARGET ${PROJECT_NAME} PROPERTY DEBUG_POSTFIX d)

add_executable(example1 src/example1.cpp)
target_link_libraries(example1 ${PROJECT_NAME})

add_executable(example2 src/example2.cpp)
target_link_libraries(example2 ${PROJECT_NAME})

add_executable(texample1 src/texample1.cpp)
target_link_libraries(texample1 ${PROJECT_NAME})

add_executable(texample2 src/texample2.cpp)
target_link_libraries(texample2 ${PROJECT_NAME})

set(CMAKE_INSTALL_PREFIX ${PROJECT_SOURCE_DIR}
    CACHE PATH "where to install")

install(TARGETS ${PROJECT_NAME}
        ARCHIVE DESTINATION lib)

install(FILES include/wineggx.h include/t-pro1.h
        DESTINATION include)
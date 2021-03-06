cmake_minimum_required(VERSION 2.8)

set(PROJECT_NAME wineggx)
project(${PROJECT_NAME})

set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE INTERNAL "")

include_directories(${PROJECT_SOURCE_DIR}/include)

add_library(${PROJECT_NAME} STATIC 
	include/wineggx.h
	src/cselectwindow.h
	src/ceggx.h
	src/wineggx.cpp
	src/cselectwindow.cpp
	src/ceggx.cpp
)

set_property(TARGET ${PROJECT_NAME} PROPERTY DEBUG_POSTFIX d)

add_executable(example1 src/example1.cpp)
target_link_libraries(example1 ${PROJECT_NAME})

add_executable(example2 src/example2.cpp)
target_link_libraries(example2 ${PROJECT_NAME})

add_executable(texample1 src/texample1.cpp)
target_link_libraries(texample1 ${PROJECT_NAME})

add_executable(texample2 src/texample2.cpp)
target_link_libraries(texample2 ${PROJECT_NAME})

add_executable(ggetch-test src/ggetch-test.cpp)
target_link_libraries(ggetch-test ${PROJECT_NAME})

add_executable(string-test src/string-test.cpp)
target_link_libraries(string-test ${PROJECT_NAME})

add_executable(mouse-test src/mouse-test.cpp)
target_link_libraries(mouse-test ${PROJECT_NAME})

add_executable(mouse-test2 src/mouse-test2.cpp)
target_link_libraries(mouse-test2 ${PROJECT_NAME})

add_executable(mouse-test3 src/mouse-test3.cpp)
target_link_libraries(mouse-test3 ${PROJECT_NAME})

add_executable(mouse-test4 src/mouse-test4.cpp)
target_link_libraries(mouse-test4 ${PROJECT_NAME})

add_executable(ver2-test src/ver2-test.cpp)
target_link_libraries(ver2-test ${PROJECT_NAME})

set(INSTALL_PREFIX ${PROJECT_SOURCE_DIR}
    CACHE PATH "where to install")
set(CMAKE_INSTALL_PREFIX ${INSTALL_PREFIX} CACHE INTERNAL "")

install(TARGETS ${PROJECT_NAME}
        ARCHIVE DESTINATION lib)

install(FILES include/wineggx.h include/t-pro1.h
        DESTINATION include)

if(MSVC)
  install(FILES ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.dir/Debug/${PROJECT_NAME}.pdb
    DESTINATION lib
    CONFIGURATIONS Debug
    )
endif(MSVC)

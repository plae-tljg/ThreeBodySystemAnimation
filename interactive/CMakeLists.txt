cmake_minimum_required(VERSION 3.10)
project(ThreeBodyAnimation)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 查找SFML库
find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

# 源文件
set(SOURCES
    main.cpp
    three_body.h
)

# 创建可执行文件
add_executable(${PROJECT_NAME} ${SOURCES})

# 链接SFML库
target_include_directories(${PROJECT_NAME} PRIVATE ${SFML_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} 
    sfml-graphics
    sfml-window
    sfml-system
)


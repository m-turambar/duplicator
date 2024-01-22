project(d3d)

cmake_minimum_required(VERSION 3.24)

set(CMAKE_CXX_STANDARD 17)

add_executable(d3d
    main.cpp
    capturer.cpp
    window.cpp
)

target_link_libraries(d3d PUBLIC
    D3D11
    DXGI
    user32
)
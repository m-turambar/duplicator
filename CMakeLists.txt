project(d3d)

cmake_minimum_required(VERSION 3.24)

set(CMAKE_CXX_STANDARD 17)

add_executable(d3d
    main.cpp
    duplicator.cpp
    window.cpp
    render.cpp
)

target_link_libraries(d3d PUBLIC
    D2D1
    D3D11
    DXGI
    user32
)
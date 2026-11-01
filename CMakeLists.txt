cmake_minimum_required(VERSION 3.15)
project(Govee_light_controller)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CPR_USE_SYSTEM_CURL ON CACHE BOOL "" FORCE)
set(CPR_FORCE_USE_SYSTEM_CURL ON CACHE BOOL "" FORCE)

include(FetchContent)

FetchContent_Declare(cpr
    GIT_REPOSITORY https://github.com/libcpr/cpr.git
    GIT_TAG 1.11.1
)
FetchContent_MakeAvailable(cpr)

FetchContent_Declare(miniaudio
    GIT_REPOSITORY https://github.com/mackron/miniaudio.git
    GIT_TAG 0.11.22
)
FetchContent_MakeAvailable(miniaudio)

find_package(nlohmann_json REQUIRED)

add_executable(Govee_light_controller main.cc)
target_include_directories(Govee_light_controller PRIVATE ${miniaudio_SOURCE_DIR})
target_link_libraries(Govee_light_controller PRIVATE cpr::cpr nlohmann_json::nlohmann_json)
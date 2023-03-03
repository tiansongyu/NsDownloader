cmake_minimum_required(VERSION 3.7)
project(cpr_demo)

# add_compile_options(-W -Wall -Werror)
add_compile_options(-W -Wall )

find_package(cpr REQUIRED)

set(CMAKE_CXX_STANDARD 17)

file(GLOB ALLINCLUDEFILES ./src/*.h ./src/*.hpp ../NsVersion/libs/cpr/include/cpr/*.hpp ../NsVersion/include/*.hpp)
file(GLOB ALLCPPFILES ./src/*.cpp ../NsVersion/libs/cpr/cpr/*.cpp ../NsVersion/src/Downloader.cpp )
set(ALL_SOURCES
    ${ALLINCLUDEFILES}
    ${ALLCPPFILES}
)
add_executable(cpr_demo ${ALL_SOURCES})
target_link_libraries(cpr_demo PRIVATE cpr::cpr)


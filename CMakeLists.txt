cmake_minimum_required(VERSION 3.10)

# Set the project name
project(QRCodeDownloader)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Find the SFML package
find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)
find_package(CURL REQUIRED)

# Add the executable
add_executable(QRCodeDownloader main.cpp)

# Link the SFML and CURL libraries
target_link_libraries(QRCodeDownloader sfml-graphics sfml-window sfml-system ${CURL_LIBRARIES})

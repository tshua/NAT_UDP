cmake_minimum_required (VERSION 2.8.4)
project (NAT_UDP_Punch CXX)
add_compile_options(-Wall)
message (${CMAKE_HOST_SYSTEM_NAME})
add_subdirectory(common)
if (${CMAKE_HOST_SYSTEM_NAME} STREQUAL Linux OR ${CMAKE_HOST_SYSTEM_NAME} STREQUAL MSYS)
add_subdirectory(unix_client)
add_subdirectory(unix_server)
target_include_directories(unix_client 
PUBLIC ./common/
PUBLIC ./common/unix
)
target_include_directories(unix_server
PUBLIC ./common/
PUBLIC ./common/unix
)
elseif (${CMAKE_HOST_SYSTEM_NAME} STREQUAL Windows)
add_subdirectory(windows_client)
target_include_directories(windows_client 
PUBLIC ./common/
PUBLIC ./common/windows
)
endif ()

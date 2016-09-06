add_library(unix OBJECT sock.cpp)
target_include_directories(unix 
PUBLIC ../
)

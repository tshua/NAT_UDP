add_library (windows OBJECT NetDaemon.cpp WinSock.cpp)
target_include_directories(windows 
PUBLIC ../
)
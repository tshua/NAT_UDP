add_executable(unix_server main.cpp server.cpp server_message_parser.cpp)
target_link_libraries(unix_server common)

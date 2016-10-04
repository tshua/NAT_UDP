add_executable(unix_client main.cpp unix_client.cpp unix_client_message_parser.cpp)
target_link_libraries(unix_client common)

add_subdirectory(unix)
add_library(common STATIC $<TARGET_OBJECTS:unix>)

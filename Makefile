CC = gcc
CXX = g++
CFLAGS = -Wall
CXXFLAGS = $(CFLAGS)

include common/Makefile
include unix_client/Makefile
include unix_server/Makefile


unix_client:$(client) $(common)
	CXX -o unix_client $(client) $(common)
unix_server:$(server) $(common)
	CXX -o unix_server $(server) $(common)

all:unix_client unix_server





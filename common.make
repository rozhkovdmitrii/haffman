CXX ?= g++
LNK ?= ar

LNK_FLAGS = rsc
CXXFLAGS = -MP -MMD -std=c++14 -Wpedantic -g
INCLUDES = -I include/ -I /usr/local/include -IHaffmanLib -IHaffmanLib/HaffmanTree

RM = rm
RMDIR = rm -rf

PROG = main.cpp
TARGET = proj
HEADER =
MAKEFILE = makefile
CXX = g++
CXX_VERSION = -std=c++17
CXX_OPTION = -Wall

UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S), Linux)
LIB_FS = -lstdc++fs
else
LIB_FS =
endif

${TARGET} : ${PROG} ${HEADER} ${MAKEFILE}
	${CXX} ${CXX_VERSION} ${CXX_OPTION} -o ${TARGET} ${HEADER} ${PROG} ${LIB_FS}

run :
	./proj

clean :
	rm -r ${TARGET}

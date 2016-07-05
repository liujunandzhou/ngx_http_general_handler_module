# set CPUS for Linux or FreeBSD

CXX := g++
CC := gcc
# add -DSNAPPY switch to snappy compress

CXXFLAGS := -g3 -O2 -fno-strict-aliasing -Wall  -fPIC -Wno-invalid-offsetof \
	-isystem /usr/local/include \
	-isystem ../lib/boost/include  \
	-I../../3rd_party/install/curl/include \
	-isystem /usr/include \
	-I../../3rd_party/jsoncpp/include \
	-I../../3rd_party/qbus/include \
	-I../include

LDFLAGS := -pthread \
    -L/home/s/lib \
	-L../../3rd_party/install/thrift/lib \
	-L../../3rd_party/install/curl/lib \
	-L/usr/lib64/ \
	-L/usr/lib/ \
	-L../../3rd_party/jsoncpp/lib \
	-L../../3rd_party/qbus/lib \
	-L.

LIBS := 

SRC := $(wildcard *.cc)
OBJ := $(patsubst %.cc, %.o, $(SRC))

DEF := -DHAVE_NETINET_IN_H -DHAVE_INTTYPES_H

TARGET := general_handler.so


target: $(TARGET)


$(TARGET): $(OBJ)
	$(CXX) $^ -o $@  $(LDFLAGS) $(LIBS) $(DEF) -shared

%.o : %.cc
	$(CXX) -c  $(CXXFLAGS) $(DEF) $< -o $@

clean:
	-rm -rf $(OBJ)
	-rm -rf $(TARGET)

.PHONY: all target clean

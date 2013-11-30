CXX      = g++
DEBUG    = -g
CXXFLAGS = -Werror -Wall -I. -Isgf
LDFLAGS  = 
LINK     = $(CXX)

TARGET = flg-auto
OBJS = main.o sgf/sgf.o
SRCS = main.cxx sgf/sgf.c

.SUFFIXES: .o .cxx .c

all: $(OBJS)
	$(LINK) $(LDFLAGS) $(OBJS) -o $(TARGET)

%.o: %.cxx
	$(CXX) $(CXXFLAGS) $(DEBUG) -c $< -o $@

%.o: %.c
	$(CXX) $(CXXFLAGS) $(DEBUG) -c $< -o $@

install: ${TARGET}
	sudo cp ${TARGET} /usr/local/bin	

clean:
	rm -f ${OBJS} 2> /dev/null
	rm -f $(TARGET) 2> /dev/null

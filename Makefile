CC=g++

TARGET=target/argparse.a
HEADER=src/argparse.hh
OBJECT=target/argparse.o
SOURCES=src/argparse.cpp
INSTALL_HEADER=/usr/include/argparse.hh
INSTALL_ARCHIVE=/usr/lib/libargparse.a


all: target $(TARGET)


$(OBJECT): $(SOURCES) $(HEADER)
	$(CC) -c $< -o $@


target:
	mkdir target


$(TARGET): $(OBJECT)
	ar rcs $@ $^


clean:
	rm -f target/*


install: $(TARGET)
	sudo cp -f $< $(INSTALL_ARCHIVE)
	sudo cp -f $(HEADER) $(INSTALL_HEADER)


uninstall:
	sudo rm -f $(INSTALL_ARCHIVE)
	sudo rm -f $(INSTALL_HEADER)

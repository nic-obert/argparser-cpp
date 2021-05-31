CC=g++

TARGET=target/argparser.a
HEADER=src/argparser.hh
OBJECT=target/argparser.o
SOURCES=src/argparser.cpp
INSTALL_HEADER=/usr/include/argparser.hh
INSTALL_ARCHIVE=/usr/lib/libargparser.a


all: target $(TARGET)


$(OBJECT): $(SOURCES) $(HEADER)
	$(CC) -c $< -o $@


target:
	mkdir target


$(TARGET): $(OBJECT)
	ar rcs $@ $^


test:
	g++ test/test.cpp -largparser -o test/test


clean:
	rm -f target/*


install: $(TARGET)
	sudo cp -f $< $(INSTALL_ARCHIVE)
	sudo cp -f $(HEADER) $(INSTALL_HEADER)


uninstall:
	sudo rm -f $(INSTALL_ARCHIVE)
	sudo rm -f $(INSTALL_HEADER)

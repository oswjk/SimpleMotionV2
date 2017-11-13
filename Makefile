SOURCES = $(wildcard *.c) \
	drivers/serial/pcserialport.c \
	drivers/tcpip/tcpclient.c

OBJECTS = $(SOURCES:%.c=%.o)

CPPFLAGS = -I.
CFLAGS = -Wall -Wextra

libsimplemotionv2.a: $(OBJECTS)
	ar rcs $@ $^

.PHONY: clean
clean:
	rm -f $(OBJECTS)

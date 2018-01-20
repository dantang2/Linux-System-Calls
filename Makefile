TARGET = cs300_testapp
OBJS = cs300_testapp.o
CFLAGS = -std=c99 -g -Wall -Werror -D_GNU_SOURCE -static
CC_C = gcc

all: clean $(TARGET) transfer

%.o: %.c
	$(CC) -c $(CFLAGS) $<

$(TARGET): $(OBJS)
	$(CC_C) $(CFLAGS) $(OBJS) -o $@

clean:
	rm -f $(TARGET)
	rm -f $(OBJS)

transfer:
	scp -P 2222 $(TARGET) root@localhost:$(TARGET)

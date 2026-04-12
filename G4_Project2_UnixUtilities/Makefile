CC = gcc
CFLAGS = -Wall -Wextra -g

TARGETS = custom_wc custom_cat custom_grep custom_ls custom_mv customfind

all: $(TARGETS)

custom_wc:
	$(CC) $(CFLAGS) commands/custom_wc.c -o custom_wc

custom_cat:
	$(CC) $(CFLAGS) commands/custom_cat.c -o custom_cat

custom_grep:
	$(CC) $(CFLAGS) commands/custom_grep.c -o custom_grep

custom_ls:
	$(CC) $(CFLAGS) commands/custom_ls.c -o custom_ls

custom_mv:
	$(CC) $(CFLAGS) commands/custom_mv.c -o custom_mv

customfind:
	$(CC) $(CFLAGS) commands/customfind.c -o customfind

clean:
	rm -f $(TARGETS)

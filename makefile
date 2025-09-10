CC = cc
CFLAGS = -Wall -Wextra -O0

tor: *.c
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm -f tor
run :
	./tor

CC := gcc
CFLAGS := -g -fPIC -O2

.c.o:
	$(CC) $(CFLAGS) -c -o $@ $<

libnkf.so: nkf.o
	$(CC) $(CFLAGS) -shared -o libnkf.so nkf.o

nkf: libnkf.so nkf.go
	@go build nkf.go

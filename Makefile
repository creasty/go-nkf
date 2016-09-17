CC := gcc
CFLAGS := -fPIC -O2

.c.o:
	$(CC) -c $(CFLAGS) -o $@ $<

nkf: nkf.o nkf.go
	@go build nkf.go

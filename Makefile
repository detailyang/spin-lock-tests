CFLAGS := -lpthread
all: xchg cas mcs k42

xchg: spin.c xchg.h
	$(CC) $< -o $@ -D XCHG $(CFLAGS)

cas: spin.c cas.h	
	$(CC) $< -o $@ -D CAS $(CFLAGS)

mcs: spin.c mcs.h	
	$(CC) $< -o $@ -D MCS $(CFLAGS)

k42: spin.c k42.h	
	$(CC) $< -o $@ -D K42 $(CFLAGS)

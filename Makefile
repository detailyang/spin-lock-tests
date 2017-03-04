CFLAGS := -lpthread
TARGETS := xchg xchg-backoff cas mcs k42

all: $(TARGETS)

xchg: spin.c xchg.h
	@$(CC) $< -o $@ -D XCHG $(CFLAGS)

xchg-backoff: spin.c xchg-backoff.h
	@$(CC) $< -o $@ -D XCHG_BACKOFF $(CFLAGS)

cas: spin.c cas.h
	@$(CC) $< -o $@ -D CAS $(CFLAGS)

mcs: spin.c mcs.h
	@$(CC) $< -o $@ -D MCS $(CFLAGS)

k42: spin.c k42.h
	@$(CC) $< -o $@ -D K42 $(CFLAGS)

test:
	@bash ./run_tests.sh

clean:
	@rm -rf $(TARGETS)

.PHONY: test clean

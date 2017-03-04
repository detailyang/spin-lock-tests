#! /bin/bash


ALGO=(cas xchg xchg-backoff mcs k42)

for spin in "${ALGO[@]}"; do
	result=()
	for i in 1 2 4 8 16; do
		t=$(./$spin $i)
		result+=("$t")
	done
	printf "%-8s" "$spin"
	for value in "${result[@]}"; do 
	    printf "%s\n" "${value}"
	done | column
done


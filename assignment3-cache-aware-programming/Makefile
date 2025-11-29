BIN=mat_mul
CC=g++

tiling: mat_mul
	g++ mat_mul.c -D OPTIMIZE_TILING -o $(BIN)/$@

loop: mat_mul
	g++ mat_mul.c -D OPTIMIZE_LOOP_OPT -o $(BIN)/$@

combination: mat_mul
	g++ mat_mul.c -D OPTIMIZE_COMBINED -o $(BIN)/$@

clean:
	@rm -rf $(BIN)

mat_mul:
	@mkdir -p $(BIN)

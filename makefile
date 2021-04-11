OLD := MMult_4x4_10
NEW := MMult_4x4_12

#
#sample makefile
#

CC := gcc
LINKER := $(CC)

# -O2 开启优化, -Wall警告信息, -msse3 sse指令识别
CFLAGS := -O2 -Wall -msse3
#连接libm数学库 libm.a
LDFLAGS := -lm

UTIL := copy_matrix.o \
		compare_matrices.o \
		random_matrix.o \
		dclock.o \
		REF_MMult.o \
		print_matrix.o

SRC=$(wildcard ./src/$(NEW).c)
SRC_OBJ=$(patsubst %.c,%.o,$(SRC))

#TEST_OBJS := test_MMult.o $(NEW).o
TEST_OBJS := test_MMult.o $(SRC_OBJ)

# $< represent the first dependency file
# $@ represent the target file
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@
#makefile 静态模式+自动化变量，语法如下
#<targets>:<target-pattern>:<prereq-patterns...>
#<commands>
all:
	make clean;
	echo "$(SRC_OBJ)"
	make test_MMult.x

test_MMult.x:$(TEST_OBJS) $(UTIL) parameters.h
	$(LINKER) $(TEST_OBJS) $(UTIL) $(LDFLAGS) \
	$(BLAS_LIB) -o $(TEST_BIN) $@

run:
	make all
	export OMP_NUM_THREADS=1
	export GOTO_NUM_THREADS=1
	echo "version='$(NEW)';" > output_$(NEW).m
	./test_MMult.x >> output_$(NEW).m
	cp output_$(OLD).m output_old.m
	cp output_$(NEW).m output_new.m

clean:
	rm -f *.o ./src/*.o *~ core *.x

cleanall:
	rm -f *.o *~ core *.x output*.m *.eps *.png
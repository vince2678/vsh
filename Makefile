%.o: %.c
	gcc -c $^

sh: sh.o
	gcc $^ -o $@

#verbose: hash_table.o hash_table_test.c
#	gcc -DVERBOSE $^ -o hash_table_test

all: sh

clean:
	rm -f sh *.o

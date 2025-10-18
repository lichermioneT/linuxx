libmymath.a:myadd.o mysub.o

	ar -rc $@ $^
myadd.o:myadd.c
	gcc -c $^

mysub.o:mysub.c
	gcc -c $^

.PHONY:output
output:
	mkdir -p mylib/include
	mkdir -p mylib/lib    
	cp -f *.a mylib/lib
	cp -f *.h mylib/include

.PHONY:clean
clean:
	rm *.o libmymath.a -rf

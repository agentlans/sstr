.PHONY: clean

libsstr.so: sstr.c sstr.h
	$(CC) -fPIC -shared sstr.c -o libsstr.so

example: example.c libsstr.so
	$(CC) example.c ./libsstr.so -o example

clean:
	rm -f libsstr.so example


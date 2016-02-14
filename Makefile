all: tokenizer.c
	gcc -g -o tokenizer tokenizer.c

run:
	./tokenizer "   012234234aabdfbdfb  a   12  asdd1 123a 0.1   123a.   0xABC323123acefd123abc     1.5e-124342341a   0xa134234abcQefa123  0xa341234234abc21312Qefa123 "

run1:
	./tokenizer "012234234aabdfbdfb  a   12  asdd1 123a 0.1   123a.   0xABC323123acefd123abc     1.5e-124342341a   0xa134234abcQefa123  0xa341234234abc21312Qefa123 "

clean:
	rm -rf *.o tokenizer

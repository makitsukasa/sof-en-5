all: a.out

a.out: *.c *.h
	gcc *.c -Wall -std=c89 -o a.out

run:
	make all
	./a.out

clean:
	rm *.out

git:
	make clean
	make all
	git commit -a -m hoge
	git push
	
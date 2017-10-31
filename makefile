all: a.out

a.out: *.c *.h
	#gcc *.c -Wall -std=c89 -o a.out -Wno-unused-variable
	gcc *.c -Wall -std=c89 -o a.out

run:
	make all
	./a.out

clean:
	rm *.out

gitcommit:
	git add -A
	git commit -a -m hoge
	
gitpush:
	git commit -a -m hoge
	git push
	
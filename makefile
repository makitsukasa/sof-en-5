all:
	gcc *.c -Wall -std=c89 -o a.out

clean:
	rm *.o

git:
	git commit -a -m hoge
	git push
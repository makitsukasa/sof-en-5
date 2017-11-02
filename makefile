all: a.out

a.out: *.c *.h
	@#gcc *.c -Wall -std=c89 -o a.out -Wno-unused-variable
	@gcc *.c -Wall -std=c89 -o a.out

ARG = in.mpl

run:
	@make all
	@./a.out ${ARG}

clean:
	rm *.out

gitcommit:
	git add -A
	git commit -a -m hoge
	
gitpush:
	-make gitcommit
	git push

gitpull:
	git pull origin master
	
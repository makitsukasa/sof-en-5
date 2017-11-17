IN=in.mpl
F=scan

all: a.out

a.out: *.c *.h
	@gcc *.c -Wall -Wextra -std=c89 -o a.out

run: a.out
	@./a.out ${IN}

clean:
	rm *.out *.gcov *.gcda *.gcno

gcovinit:
	-make clean
	gcc *.c -Wall -Wextra -std=c89 -o a.out -O0 -coverage

gcovrun:
	-./a.out ${IN}
	-gcov ${F}.gcda
	cat ${F}.c.gcov > out.txt

gcovall:
	$(foreach VAR,$(wildcard in_*.mpl),$(make gcovrun IN=${VAR}))

gitcommit:
	git add -A
	git commit -a -m hoge
	
gitpush:
	-make gitcommit
	git push

gitpull:
	git pull origin master
	
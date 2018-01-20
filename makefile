IN=in.mpl
F=nodedata

.PHONY:
	all run clean

all: a.out

a.out: *.c *.h
	@gcc *.c -Wall -Wextra -std=c89 -o a.out -Wno-unused-parameter -Wno-implicit-function-declaration

run: a.out
	@./a.out ${IN}

clean:
	-rm *.out *.gcov *.gcda *.gcno

gdb:
	make clean
	gcc *.c -Wall -Wextra -std=c89 -o a.out -O0 -g
	gdb ./a.out

gcovinit:
	make clean
	gcc *.c -Wall -Wextra -std=c89 -o a.out -O0 -coverage

gcovrun:
	-./a.out ${IN} > ${IN}.txt
	-gcov ${F}.gcda
	cat ${F}.c.gcov > out.txt

#gcovall:
#	$(foreach VAR,$(wildcard in_*.mpl),$(make gcovrun IN=${VAR}))

gitcommit:
	git add -A
	git commit -a -m hoge
	
gitpush:
	-make gitcommit
	git push

gitpull:
	git pull origin master
	
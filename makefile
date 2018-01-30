IN=in.mpl
F=compiler instruction-words

.PHONY:
	all run clean

all: a.out

a.out: *.c *.h
	gcc *.c -Wall -Wextra -std=c89 -o a.out

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
	#-./a.out ${IN} > ${IN}.txt
	-./a.out ${IN}
	#-gcov ${F}.gcda
	-gcov -b ${F}.gcda

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
	
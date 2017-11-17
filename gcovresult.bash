if [$# -lt 1]; then
	gcov scan.gcda
	cat scan.c.gcov > out.txt
else
	gcov ${1}.gcda
	cat ${1}.c.gcov > out.txt
fi
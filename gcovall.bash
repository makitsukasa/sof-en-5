for textfile in $( ls in_*.mpl ); do
	#echo "${textfile}"
	if [$# -lt 1]; then
		make gcovrun IN=${textfile} F=scan
	else
		make gcovrun IN=${textfile} F=${1}
	fi
done

if [$# -lt 1]; then
	make gcovrun IN="" F=scan
	make gcovrun IN="hogerahogehogehogehogera.hoge" F=scan
else
	make gcovrun IN="" F=${1}
	make gcovrun IN="hogerahogehogehogehogera.hoge" F=${1}
fi
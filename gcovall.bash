for textfile in $( ls *.mpl ); do
	make gcovrun IN=${textfile}
done
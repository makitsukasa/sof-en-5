for textfile in $( ls in*.mpl ); do
  #echo "${textfile}"
  make gcovrun IN=${textfile}
done
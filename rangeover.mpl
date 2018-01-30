program rangeover;
var index : integer;
var arr : array [10] of integer;
begin
	index := 0;
	while true do
	begin
		arr[index] := index;
		writeln('arr[', index : 2, '] is ', arr[index]);
		index := index + 1;
	end
end.
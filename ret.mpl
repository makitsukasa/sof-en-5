program ret;
procedure proc(x : integer);
begin
	x := 3;
	return;
	x := 5;
end;
var a : integer;
begin
	a := 0;
	call proc(a);
	writeln('a is ', a);
end.

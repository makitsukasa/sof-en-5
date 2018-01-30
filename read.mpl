program readdddd;
var	i, j, k : integer;
	c, d, e : char;

procedure read3int(a, b, c : integer);
begin
	readln(a, b, c);
end;

begin
	call read3int(i, j, k);
	read(c, d, e);
	writeln(i, j, k, c, d, e);
end.
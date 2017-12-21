program test;
var
	i : integer;
	a : array[100] of char;
	c : char;
	b, b2 : boolean;

procedure proc0;
	var x, y : integer;
	begin
		if x = 1 then a := 'a';
		if x > 2 then a := 'b' else a := 'c';
		if true or false and true then a := 'd';
		while x > 0 do x := x - 1
	end;

procedure proc1(c : char);
	begin
		readln;
		read(c);
		read(c, i);
		writeln;
		writeln('asfadf');
		writeln(c:3, i);
	end;

procedure proc2(c1, c2, c3 : char);
	begin
		if not (boolean(c2)) then writeln(c3);
	end;

begin
	call proc0;
	call proc0;
	call proc0;
	call proc0;
	call proc1(c);
	call proc2(a[1], a[i], a[1 + 3 * 3]);
end.
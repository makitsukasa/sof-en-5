program paramarg;

procedure proc0;
var a : integer;
begin
	a := 0;
	writeln(a);
end;

procedure proc1(p1 : integer);
var a : integer;
begin
	a := p1;
	writeln(a);
	p1 := 100;
end;

procedure proc2(p1, p2 : integer);
var a : integer;
begin
	a := p1;
	write(a, ' ');
	a := p2;
	writeln(a);
	p1 := 200;
	p2 := 200;
end;

procedure proc3(p1, p2, p3 : integer);
var a : integer;
begin
	a := p1;
	write(a, ' ');
	a := p2;
	write(a, ' ');
	a := p3;
	writeln(a);
	p1 := 300;
	p2 := 300;
	p3 := 300;
end;

var x, y : integer;
var arr : array [5] of integer;

begin
	x := 45;
	y := 5;
	arr[3] := 21;
	
	call proc0;
	call proc1(arr[3]);
	call proc2(x div y, arr[3]);
	call proc3(x, arr[3],
				integer(x <> y));
	call proc3(x, y, arr[3]);
	
end.

program hogeprog;
var
	hoge1, piyo : boolean;
	hoge2, piyo2 : array[7] of integer;
procedure hogeproc1(hoge1, piyo, huga : integer);
	var hoge4 : integer;
	begin hoge1 := 1; end;
procedure hogeproc2;
	var hoge6 : integer;
	begin writeln; end;
begin
	call hogeproc1(1, 2, 3);
	call hogeproc1(hoge1);
	call hogeproc2;
end.
program typeconv;
 var i : integer; b : boolean; c : char;
begin
  i := integer(false);
  while i <= integer(true) do begin
    writeln( boolean(i), ' : ', i);
    i := i + 1;
  end;
  writeln;
  i := integer(' ');
  while i < 127 do begin
    if i div 16 * 16 = i then writeln;
    write(char(i), ' ');
    i := i + 1;
  end;
  writeln
end.
  
program hogeprog;
var
	hoge1, piyo : char;
	hoge2, piyo2 : array[7] of integer;
procedure hogeproc1(hoge1, piyo, huga : integer);
	var hoge4 : integer;
	begin hoge1 := 1; end;
procedure hogeproc2;
	var hoge6 : integer;
	begin writeln(hoge6); end;
begin

end.
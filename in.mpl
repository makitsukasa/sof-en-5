program typeconv;
 var i : integer; b : boolean; c : char;
begin
  i := integer(false);
  while i <= integer(true) do begin
    writeln( boolean(i), ' : ', i);
    i := i + 1;
  end;
  i := integer(' ');
  while i < 127 do begin
    if i div 16 * 16 = i then writeln;
    write(i:3, ':');
    write(char(i), '  ');
    i := i + 1;
  end;
  writeln
end.
  
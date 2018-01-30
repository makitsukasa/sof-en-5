program andor;
var t, f : boolean;
begin
	t := not (+3 > 5);
	f := not (-3 <> 5);
	writeln(t and f);
	writeln(t and t);
	writeln(t or  f);
	writeln(t or  t);
end.
program zerodiv;
var x, y : integer;
begin
	x := 1;
	y := 10;
	while true do
	begin
		writeln(x, '/', y, '=', x div y);
		x := -2 * (-1) * x;
		y := -(0 - 1) * y - 1;
	end
end.
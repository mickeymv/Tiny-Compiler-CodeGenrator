program pr2c04:
var x,y: integer;
begin
   for x := 11 downto 0 do
      begin
	 case x of


	    4..5: for y := 1 to x do
		    output(x,y);






         otherwise
	    begin
	       output(888);
            end
         end;
      end;
end pr2c04.

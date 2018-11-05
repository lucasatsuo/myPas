{ test on functionalities }

program my_program; { mypas() }

{ header() }
var { varmodel() }
    a, b, c : boolean ;
    i, j, k : real ;
    t, u, v : integer ;
    x, y, z : double ;

{ procmodel() }
procedure proc_name(var a, b, c : integer) ;{ formalparms() }
    var x, y, z : double;
    procedure proc1_name;
    begin
    end;
begin
end;

{ funcmodel() }
function func_name(j, k, l : real) : integer ;
begin
end;

{ body() }
begin
    if not a = -3 then { ifstmt() }
        while 13.2e14 >= true do { whlstmt() }
            var1 := var2 := (not a and b)
    else
        repeat { repstmt() }
            begin
                begin end;
                begin end;
            end;
            begin
            end;
            begin
            end;
        until not pi * newton(a>b, 12<44, not 32.2)
end.

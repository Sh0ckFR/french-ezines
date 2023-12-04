// Boolean variables are set to False when
// they are compiled, so we can think of that
// fact as of the code not been Initialized.
var
  Initialized: Boolean;

procedure Initialize;
begin
  Initialized := True;
  // Initialization code here
end;

begin
  if not Initialized then Initialize;
  // The main renaming code here
end.
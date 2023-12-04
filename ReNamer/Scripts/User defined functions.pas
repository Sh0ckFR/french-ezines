procedure DoSomething(var Parameter: WideString);
begin
  // Do something here with Parameter.
  // Original passed variable will be changed,
  // because Parameter has been passed as a Var.
end;

function DoAnother(Parameter: WideString): WideString;
begin
  // Do something here with Parameter,
  // and return Result as a string.
  Result := '';
end;

var
  S: WideString;

begin
  DoSomething(S);
  FileName := DoAnother(S);
end.
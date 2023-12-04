var
  Files: TWideStringArray;

procedure Add(const S: WideString);
begin
  SetLength(Files, Length(Files)+1);
  Files[Length(Files)-1] := S;
end;

function Exists(const S: WideString): Boolean;
var I: Integer;
begin
  Result := False;
  for I:=0 to Length(Files)-1 do
    if WideSameText(Files[I], S) then
      begin Result := True; Break; end;
end;

var
  NewFileName: WideString;
  Counter: Integer;

begin
  Counter := 2;
  NewFileName := FileName;
  while Exists(NewFileName) do
    begin
    NewFileName := 
      WideExtractBaseName(FileName) +
      ' (' + IntToStr(Counter)+')' + 
      WideExtractFileExt(FileName);
    Counter := Counter + 1;
    end;
  FileName := NewFileName;
  Add(FileName);
end.
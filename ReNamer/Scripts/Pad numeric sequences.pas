const
  PAD_TO_LENGTH = 3;

function CountDigits(const S: WideString; StartI: Integer): Integer;
var I: Integer;
begin
  Result := 0;
  for I:=StartI to WideLength(S) do
  begin
    if IsWideCharDigit(S[I]) then
      Inc(Result)
    else
      Break;
  end;
end;

function MakeZeros(Count: Integer): WideString;
var I: Integer;
begin
  Result := '';
  for I:=1 to Count do
    Result := Result + '0';
end;

var
  Start, Count: Integer;

begin
  Start := 1;
  while Start < WideLength(FileName) do
  begin
    Count := CountDigits(FileName, Start);
    if (Count > 0) then
      if (Count < PAD_TO_LENGTH) then
      begin
        WideInsert(MakeZeros(PAD_TO_LENGTH-Count), FileName, Start);
        Start := Start + PAD_TO_LENGTH;
      end
      else
        Start := Start + Count
    else    
      Inc(Start);
  end;
end.

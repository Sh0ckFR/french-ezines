function GetTickCount: Longint;
  external 'GetTickCount@kernel32.dll stdcall';

procedure Sleep(Milliseconds: Cardinal);
  external 'Sleep@kernel32.dll stdcall';

begin
  FileName := IntToStr(GetTickCount);
  Sleep(10);
end.
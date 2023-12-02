Program MCD_5_01_KEYGEN;

Uses
  Dos,Crt;

Var
  EAX,EDX,ESI:integer;
  EBX:String;

Begin
  EAX:=0;
  EBX:='';
  EDX:=0;
  ESI:=0;
  Clrscr;
  Writeln('');
  Writeln(' Media Changer Deluxe V5.01 Key Generator by Blackhawk/MFD ');
  Writeln('');
  Write('   Enter your registration name : ');
  readln(EBX);
  if EBX = '' then
  Begin
    Writeln('   You must enter your name...');
    Writeln('');
    Writeln(' Press a key to exit');
    repeat
    until keypressed;
    clrscr;
    Exit;
  End;
  For EDX:=1 to length(EBX) do
  Begin
    EAX:=ord(EBX[EDX]);
    EAX:=EAX*2+EAX;
    ESI:=EAX*2+ESI;
    EAX:=0;
  End;
  Write('   Your registration number     : ');
  Write(esi);
  Writeln('');
  Writeln('');
  Writeln(' Press a key to exit');
  repeat
  until keypressed;
  clrscr;
End.
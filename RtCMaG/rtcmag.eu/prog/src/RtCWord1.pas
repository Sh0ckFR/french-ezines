uses crt,dos;

var
  Source,NewFile:text;
  c:char;
  SourceName,NewFileName,Chaine:string;

begin
  clrscr;
  writeln('<<<RTCWord 1.01>>>  by Androgyne from the RTCG4ng');
  writeln('');
  if ParamCount=2 then begin
    assign(Source,ParamStr(1));
    assign(NewFile,ParamStr(2));
  end else begin
    repeat
      write('> source file      : ');
      readln(SourceName)
    until (SourceName<>'');
    assign(Source,SourceName);
    repeat
      write('> destination file : ');
      readln(NewFileName)
    until (NewFileName<>'');
    assign(NewFile,NewFileName);
  end;
  {$I-}
  reset(Source);
  {$I+}
  if IOResult<>0 then begin
    writeln('Error ! Source file does not exist !');
    halt;
  end;
  rewrite(NewFile);
  while not (eof(Source)) do begin
    chaine:='';
    read(Source,c);
    while ((Upcase(c) in ['A'..'Z']) or
    (ord(c) in [$E0,$E2,$E4,$E9,$E8,$EA,$EB]) or
    (ord(c) in [$EC,$EE,$EF,$F2,$F4,$F6,$F9,$FB,$FC]) or
    (ord(c) in [$2C,$2E,$3B,$3A,$21,$3F,$20]) or
    (ord(c) in [$E7,$27,$22,$2D,$B0,$28,$29,$2F,$5C]) or
    (ord(c) in [$30,$31,$32,$33,$34,$35,$36,$37,$38,$39])) do begin
      Chaine:=Chaine+c;
      read(Source,c);
    end;
    if (length(chaine)>2) then begin
      writeln(NewFile,Chaine);
      writeln(Chaine);
    end;
  end;
  close(Source);
  close(NewFile);
  writeln('OK ! That''s all folks !');
end.
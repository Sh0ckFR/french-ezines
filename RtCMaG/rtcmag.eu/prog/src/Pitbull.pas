uses crt,dos,textscr;

const
  max_entries=3000;
  max_cfg=10;

type
  Logrec_type=record
    name:string[200];
    time:longint;
    size:longint;
    checksum:longint;
    found:boolean;
    end;
  Cfg_type=string[6];
  TChoice=record
    name:string;
    value:byte;
    active:boolean;
  end;



var
  LstFile:text;

  LogFile:file of Logrec_type;
  LogEntries:longint;
  Log:array [1..max_entries] of ^Logrec_type;

  i,j:word;
  SearchDir:string;
  CurrDir:string;

  CfgFile:file of Cfg_type;
  CfgEntries:byte;
  Cfg:array [1..max_cfg] of ^Cfg_type;

function SearchLog(fname:string;var FN:word):boolean;
var j:word;
begin
  fname:=UpString(fname);
  if LogEntries>0 then for j:=1 to LogEntries do begin
    if fname=Log[j]^.Name then begin
      SearchLog:=true;
      FN:=j;
      exit;
    end;
  end;
  SearchLog:=false;
end;

function Get_Checksum(FName:string):longint;
var
  F:file;
  cs:longint;
  j,x:integer;
  buf:array [0..511] of byte;
begin
  cs:=0;
  assign(F,FName);
  {$I-}
  reset(F,1);
  {$I+}
  if IOResult<>0 then begin
    Get_Checksum:=0;
    textcolor(14);
    Writeln('Ouaf! Ouaf! No checksum for this file!');
    textcolor(7);
    exit;
  end;
  repeat
    blockread(F,buf,512,x);
    if x>0 then for j:=0 to x-1 do cs:=cs+buf[j];
  until eof(F);
  close(F);
  Get_Checksum:=cs;
end;

procedure check_dir(dir:string);
  var
    SR:SearchRec;
    Checksum:longint;
    FN,i:word;
    cmd:char;
  procedure check_files(ext:string);
  begin
    FindFirst(dir+ext,AnyFile,SR);
    while DosError=0 do begin
      if SearchLog(dir+SR.name,FN) then begin
        Checksum:=Get_Checksum(dir+SR.name);
        if (Log[FN]^.time<>SR.time) or (Log[FN]^.size<>SR.size) or (Log[FN]^.checksum<>Checksum) then begin
          textcolor(14);
          Writeln('Ouaf! Oauf! '+dir+SR.name+' has changed!');
          Writeln(' Do you want to update its record? ');
          textcolor(7);
          write(LstFile,'Ouaf! Ouaf! ',dir+SR.name,' has changed! Update? ');
          repeat cmd:=UpCase(Readkey) until cmd in ['Y','N'];
          if cmd='Y' then begin
            Log[FN]^.time:=SR.time;
            Log[FN]^.size:=SR.size;
            Log[FN]^.Checksum:=Checksum;
            Log[FN]^.found:=true;
          end;
          Writeln(cmd);
          writeln(LstFile,cmd);
        end else begin
          Writeln(dir+SR.name+' validated.');
          Log[FN]^.found:=true;
        end;
      end else begin
        if LogEntries<max_entries then begin
          Writeln('Ouaf! '+dir+SR.name+' in Pitbull''s log.');
          writeln(LstFile,'New file ',dir+SR.name,' in Pitbull''s log.');
          LogEntries:=LogEntries+1;
          new(Log[LogEntries]);
          Log[LogEntries]^.name:=dir+SR.name;
          Log[LogEntries]^.time:=SR.time;
          Log[LogEntries]^.size:=SR.size;
          Log[LogEntries]^.checksum:=Get_Checksum(dir+SR.name);
          Log[LogEntries]^.found:=true;
        end else begin
          textcolor(14);
          Writeln('OUAF! OUAF! TOO MANY ENTRIES. COULD NOT ADD '+dir+SR.name+'.');
          textcolor(7);
          writeln(LstFile,'TOO MANY ENTRIES. COULD NOT ADD ',dir+SR.name,'.');
        end;
      end;
      FindNext(SR);
    end;
  end;

begin
  dir:=UpString(dir);

  for i:=0 to CfgEntries do check_files(Cfg[i]^);

  FindFirst('*.*',Directory,SR);
  while DosError=0 do begin
    if (SR.attr and Directory <>0) and (SR.name[1]<>'.') then begin
      ChDir(SR.name);
      check_dir(dir+SR.name+'\');
      ChDir('..');
    end;
    FindNext(SR);
  end;
end;

procedure check_boot;
var
  FN,j:word;
  cs:longint;
  buf:array [0..511] of byte;
  r:registers;
  cmd:char;
  currdrv:byte;
begin
  r.ah:=$19;
  intr($21,r);
  currdrv:=r.al;
  if currdrv>=2 then currdrv:=currdrv+$80-2;
  if currdrv=$80 then begin
    r.ax:=$201;
    r.bx:=ofs(buf);
    r.es:=sseg;
    r.cx:=1;
    r.dx:=$80;
    intr($13,r);
    r.ax:=$201;
    intr($13,r);
    cs:=0;
    for j:=0 to 511 do cs:=cs+buf[j];
    if SearchLog('**MBS',FN) then begin
      Log[FN]^.found:=true;
      if Log[FN]^.checksum=cs then Writeln('Master Boot Sector verified.') else begin
        textcolor(14);
        Writeln('Ouaf! Ouaf! Master Boot Sector has changed! Update? ');
        textcolor(7);
        write(LstFile,'Ouaf! Ouaf! Master Boot Sector has changed! Update? ');
        repeat cmd:=UpCase(Readkey) until cmd in ['Y','N'];
        if cmd='Y' then Log[FN]^.checksum:=cs;
        Writeln(cmd);
        writeln(LstFile,cmd);
      end;
    end else begin
      Writeln('Ouaf! Master Boot Sector data ADDED to log');
      writeln(LstFile,'Master Boot Sector data ADDED to log');
      LogEntries:=LogEntries+1;
      new(Log[LogEntries]);
      Log[LogEntries]^.name:='**MBS';
      Log[LogEntries]^.checksum:=cs;
      Log[LogEntries]^.found:=true;
    end;
    j:=$1BE;
    while (j<$1FE) and (buf[j]<>$80) do j:=j+$10;
    if buf[j]=$80 then begin
      r.dx:=buf[j]+256*buf[j+1];
      r.cx:=buf[j+2]+256*buf[j+3];
    end else exit;
  end else begin
    r.cx:=1;
    r.dx:=currdrv;
  end;
  if currdrv<$81 then begin
    r.ax:=$201;
    r.bx:=ofs(buf);
    r.es:=sseg;
    intr($13,r);
    r.ax:=$201;
    intr($13,r);
    cs:=0;
    for j:=0 to 511 do cs:=cs+buf[j];
    if SearchLog('**BOOT',FN) then begin
      Log[FN]^.found:=true;
      if Log[FN]^.checksum=cs then Writeln('BootSector verified.') else begin
        textcolor(14);
        Writeln('Ouaf! Ouaf! Boot Sector has changed! Update? ');
        textcolor(7);
        write(LstFile,'Ouaf! Ouaf! Boot Sector has changed! Update? ');
        repeat cmd:=UpCase(readkey) until cmd in ['Y','N'];
        if cmd='Y' then Log[FN]^.checksum:=cs;
        Writeln(cmd);
        writeln(LstFile,cmd);
      end;
    end else begin
      Writeln('Ouaf! Boot Sector data ADDED to log.');
      writeln(LstFile,'Boot Sector data ADDED to log.');
      LogEntries:=LogEntries+1;
      new(Log[LogEntries]);
      Log[LogEntries]^.name:='**BOOT';
      Log[LogEntries]^.checksum:=cs;
      Log[LogEntries]^.found:=true;
    end;
  end;
end;

procedure PurgeFile(j:word);
var
  cmd:char;
  i:word;
begin
  textcolor(14);
  Writeln('Ouaf! Ouaf! '+Log[j]^.name+' was not found.');
  Writeln(' Delete from Pibulll''s log? ');
  textcolor(7);
  write(LstFile,'Ouaf! Ouaf! ',Log[j]^.name,' was not found. Delete from Pitbull''s log?');
  repeat cmd:=UpCase(readkey) until cmd in ['Y','N'];
  if cmd='Y' then begin
    for i:=j to LogEntries-1 do Log[i]^:=Log[i+1]^;
    LogEntries:=LogEntries-1;
  end;
  Writeln(cmd);
  writeln(LstFile,cmd);
end;


procedure OuafOuaf;
var
  j:word;
  cmd:char;
begin
  clrscr;
  writeln('Press [Space]');
  repeat cmd:=UpCase(readkey) until cmd=#32;
  SearchDir:='\';
  GetDir(0,CurrDir);
  ChDir(SearchDir);
  check_boot;
  check_dir(SearchDir);
  j:=1;
  while j<=LogEntries do begin
    if Log[j]^.found then j:=j+1 else PurgeFile(j);
  end;
  ChDir(CurrDir);
  writeln('Press [Space]');
  repeat cmd:=UpCase(readkey) until cmd=#32;
  InitScreen;
end;

function Is_in_list(ext:string):boolean;
var
 k:byte;
 test:boolean;
begin
  test:=false;
  for k:=0 to CfgEntries do test:=(Cfg[k]^=ext) or test;
  Is_in_list:=test;
end;


{ interface }


function Choice(Event:array of TChoice):byte;
const
  keys:array [0..4] of string = ('F1','F2','F3','F4','Esc');
var
  i:byte;
  cmd:char;
  menu:string;
begin
  menu:='';
  for i:=0 to 4 do if Event[i].active then
    menu:=menu+keys[i]+' '+Event[i].name+' ';
  ScreenHeader(menu);
  repeat cmd:=readkey until cmd in [#59,#60,#61,#62,#27];
  case cmd of
    #59:Choice:=Event[0].value;
    #60:Choice:=Event[1].value;
    #61:Choice:=Event[2].value;
    #62:Choice:=Event[3].value;
    #27:Choice:=Event[4].value;
  end;
end;

procedure DisplayLogo;
const
  Logo='VDS-AV.Pitbull v1.0';
  URL='URL : www.vds.fr.st - mail : vds@fr.st';
begin
  AddScreen(centered(Logo));
  AddScreen(centered(URL));
  EmptyLines(1);
end;

procedure AddExt;
var
  ext:string;
begin
  repeat
    AskScreen('Add extension : ',ext);
  until length(ext)=3;
  ext:='*.'+ext;
  if Is_in_list(ext) then begin
    clrscr;
    AddScreen(ext+' is already in Pitbull''s list.');
    Exit;
  end;
  CfgEntries:=CfgEntries+1;
  new(Cfg[CfgEntries]);
  Cfg[CfgEntries]^:=ext;
  clrscr;
  AddScreen(ext+' is added to Pitbull''s list.');
end;


procedure DelExt;
var
  ext:string;
begin
  repeat
    AskScreen('Kill extension : ',ext);
  until length(ext)=3;
  ext:='*.'+ext;
  if not Is_in_list(ext) then begin
    clrscr;
    AddScreen('Pitbull does not know this extension...');
    Exit
  end;
  for i:=0 to CfgEntries do if Cfg[i]^=ext then begin
    for j:=i to CfgEntries-1 do Cfg[j]^:=Cfg[j+1]^;
    CfgEntries:=CfgEntries-1;
    clrscr;
    AddScreen(ext+' are removed from list.');
  end;
end;



procedure MainMenu;
const
  Tab:array [0..4] of TChoice=
    ((name:'Ouaf!'  ;value:1;active:true),
     (name:'add ext';value:2;active:true),
     (name:'del ext';value:3;active:true),
     (name:'quit'   ;value:4;active:false),
     (name:'quit'   ;value:0;active:true));
var
  result:byte;
begin
  repeat
    EmptyLines(2);
    DisplayLogo;
    AddScreen(centered(' Pitbull''s list :'));
    for j:=1 to CfgEntries do AddScreen(centered(' -> '+Cfg[j]^));
    EmptyLines(1);
    result:=Choice(Tab);
    case result of
      0:exit;
      1:OuafOuaf;
      2:AddExt;
      3:DelExt;
      4:exit;
    end;
  until false;
end;


begin
  OffCursor;

  assign(LogFile,'Pitbull.DAT');
  {$I-}
  reset(LogFile);
  {$I+}
  if IOResult<>0 then LogEntries:=0 else begin
    for LogEntries:=1 to FileSize(LogFile) do begin
      new(Log[LogEntries]);
      read(LogFile,Log[LogEntries]^);
    end;
    close(LogFile);
  end;

  assign(CfgFile,'Pitbull.CFG');
  {$I-}
  reset(CfgFile);
  {$I+}
  if IOResult<>0 then CfgEntries:=0 else begin
    for CfgEntries:=1 to FileSize(CfgFile) do begin
      new(Cfg[CfgEntries]);
      read(CfgFile,Cfg[CfgEntries]^);
    end;
    close(CfgFile);
  end;

  assign(LstFile,'Pitbull.LST');
  rewrite(LstFile);

  textcolor(7);
  InitScreen;
  MainMenu;

  clrscr;

  rewrite(LogFile);
  for j:=1 to LogEntries do begin
    Log[j]^.found:=false;
    write(LogFile,Log[j]^);
  end;
  close(LogFile);

  rewrite(CfgFile);
  for j:=1 to CfgEntries do write(CfgFile,Cfg[j]^);
  close(CfgFile);

  writeln(LogEntries,' files in the current log file.');
  writeln(LstFile,LogEntries,' files in the current log file.');
  close(LstFile);

end.
uses crt,dos,textscr;


type

TChoice=record
  name:string;
  value:byte;
  active:boolean;
end;


var
  Tablo:array [0..4] of Tchoice;
  choix:Tchoice;
  Authorized:array [0..3] of boolean;

  j:byte;


{############# PROCEDURE DE GESTION DES DICOS ###################}


{CountWord : proc‚dure de comptage des mots}

procedure CountWord(SourceName:string);
var
  x:longint;
  chaine:string;
  Source:text;
begin
  AddScreen('Generating...');
  assign(Source,SourceName);
  {$I-}
  reset(Source);
  {$I+}
  if IOResult<>0 then begin
    AddScreen('Error ! Source file does not exist !');
    exit;
  end;
  x:=0;
  while not EOF(Source) do begin
    readln(Source,Chaine);
    inc(x);
  end;
  AddScreen('This file contains '+NewStr(x)+' words.');
end;


{ExtractWord : proc‚dure d'extraction des mots}

procedure ExtractWord(SourceName,NewFileName:string);
var
  Source,NewFile:text;
  chaine:string;
  c:char;
begin
  AddScreen('Generating...');
  assign(Source,SourceName);
  assign(NewFile,NewFileName);
  {$I-}
  reset(Source);
  {$I+}
  if IOResult<>0 then begin
    AddScreen('Error ! Source file does not exist !');
    exit;
  end;
  rewrite(NewFile);

  while not (EOF(Source)) do begin
    chaine:='';
    read(Source,c);
    {Aut[0] : lettres accentu‚e + ‡ + n tilde
     Aut[1] : , . ; : õ ?
     Aut[2] : ' " - ø ( ) / \
     Aut[3] : 0 1 2 3 4 5 6 7 8 9}
    while ((Upcase(c) in ['A'..'Z',' ']) or
    ((ord(c) in [$E0..$FD,$FF]) and Authorized[0]) or
    ((ord(c) in [$2C,$2E,$3B,$3A,$21,$3F]) and Authorized[1]) or
    ((ord(c) in [$27,$22,$2D,$B0,$28,$29,$2F,$5C]) and Authorized[2]) or
    ((ord(c) in [$30,$31,$32,$33,$34,$35,$36,$37,$38,$39]) and Authorized[3]))
    do begin
      Chaine:=Chaine+c;
      read(Source,c);
    end;
    if (length(chaine)>2) then writeln(NewFile,Chaine);
  end;
  close(Source);
  close(NewFile);
  AddScreen('OK!');
end;


{MixDic : proc‚dure de mixage des dicos}

procedure MixDic(Source1Name,Source2Name,NewFileName:string);
var
  Source1,Source2,NewFile:text;
  chaine1,chaine2:string;
  finished:byte;
begin
  AddScreen('Generating...');
  assign(Source1,Source1Name);
  assign(Source2,Source2Name);
  assign(NewFile,NewFileName);
  {$I-}
  reset(Source1);
  {$I+}
  if IOResult<>0 then begin
    AddScreen('Error ! Source file 1 does not exist !');
    exit;
  end;
  {$I-}
  reset(Source2);
  {$I+}
  if IOResult<>0 then begin
    AddScreen('Error ! Source file 2 does not exist !');
    exit;
  end;
  rewrite(NewFile);
  readln(Source1,Chaine1);
  readln(Source2,Chaine2);
  finished:=0;
  while (finished=0) do begin
    Chaine1:=UpString(Chaine1);
    Chaine2:=UpString(Chaine2);
    if EOF(Source1) and (Chaine1='') then finished:=1;
    if EOF(Source2) and (Chaine2='') then finished:=2;
    if (Chaine1<Chaine2) and (Chaine1<>'') then begin
      Writeln(NewFile,Chaine1);
      readln(Source1,Chaine1);
    end else if (Chaine1>Chaine2) and (Chaine2<>'') then begin
      Writeln(NewFile,Chaine2);
      readln(Source2,Chaine2);
    end else begin
      if (Chaine1<>'') then
        Writeln(NewFile,Chaine1)
      else
        Writeln(NewFile,Chaine2);
      readln(Source1,Chaine1);
      readln(Source2,Chaine2);
    end;
  end;
  if finished=2 then begin
    writeln(NewFile,Chaine1);
    while not EOF(Source1) do begin
      readln(Source1,Chaine1);
      Writeln(NewFile,Chaine1);
    end;
  end;
  if not finished=1 then begin
    writeln(NewFile,Chaine2);
    while not EOF(Source2) do begin
      readln(Source2,Chaine2);
      Writeln(NewFile,Chaine2);
    end;
  end;
  Close(Source1);
  Close(Source2);
  Close(NewFile);
  AddScreen('OK!');
end;


function SpecialStr(X:longint):string;
var S:string;
begin
  Str(X,S);
  while length(S)<3 do S:='0'+S;
  SpecialStr:=S;
end;


{DivideDic : proc‚dure de division des dicos}

procedure DivideDic(SourceName:string;Number:word);
var
  Source,Newfile:text;
  NewfileName,chaine:string;
  x,i:word;
begin
  AddScreen('Generating...');
  assign(Source,SourceName);
  {$I-}
  reset(Source);
  {$I+}
  if IOResult<>0 then begin
    AddScreen('Error ! Source file does not exist !');
    exit;
  end;
  x:=0;
  while not EOF(Source) do begin
    i:=0;
    NewFileName:=copy(SourceName,1,(length(SourceName)-3))+SpecialStr(x);
    assign(NewFile,NewFileName);
    rewrite(NewFile);
    while not EOF(Source) and (i<Number) do begin
      readln(Source,chaine);
      writeln(NewFile,chaine);
      inc(i);
    end;
    close(Newfile);
    inc(x);
  end;
  AddScreen(NewStr(x)+' dictionnaries have been created.');
end;



{################### PROCEDURES D'INTERFACE #######################}

{Choice : Event est un tableau de 5 TChoice correspondant respectivement …
 F1, F2, F3, F4 et Esc.}

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
  LogoScreen:array[0..6] of string=(
  (' /----------------------\ '),
  ('/ Androgyne/RtC presents \'),
  (' ======================== '),
  ('<----- RtCWord v2.0 ----->'),
  (' ======================== '),
  ('\   a RtCtech software   /'),
  (' \----------------------/ '));
  URL='URL : www.rtc.fr.st - mail : rtcandrogyne@yahoo.fr';
var
  i:byte;
begin
  for i:=0 to 6 do AddScreen(centered(LogoScreen[i]));
  EmptyLines(4);
  AddScreen(centered(URL));
  EmptyLines(4);
end;


function EnabledDisabled(B:boolean):string;
begin
  case B of
    true :EnabledDisabled:='enabled';
    false:EnabledDisabled:='disabled';
  end;
end;



procedure MenuCharacter;
const
  Tab:array [0..4] of TChoice=
    ((name:'stresses'     ;value:111;active:true),
     (name:'punctuation'  ;value:112;active:true),
     (name:'miscellaneous';value:113;active:true),
     (name:'digits'       ;value:114;active:true),
     (name:'exit'         ;value:110;active:true));
var
  result:byte;

begin
  ClearScreen;
  repeat
    AddScreen('');
    AddScreen('Stressed letters  : '+EnabledDisabled(Authorized[0]));
    AddScreen('Punctuation signs : '+EnabledDisabled(Authorized[1]));
    AddScreen('Miscellaneous     : '+EnabledDisabled(Authorized[2]));
    AddScreen('Digits            : '+EnabledDisabled(Authorized[3]));
    result:=Choice(Tab);
    case result of
      110:exit;
      111:Authorized[0]:=not Authorized[0];
      112:Authorized[1]:=not Authorized[1];
      113:Authorized[2]:=not Authorized[2];
      114:Authorized[3]:=not Authorized[3];
    end;
  until false;
end;


procedure MenuExtract;
const
  Tab:array [0..4] of TChoice=
    ((name:'source'     ;value:11;active:true),
     (name:'destination';value:12;active:true),
     (name:'characters' ;value:13;active:true),
     (name:'go!'        ;value:14;active:true),
     (name:'exit'       ;value:10;active:true));
var
  result:byte;
  SourceName,NewFileName:string;
begin
  SourceName:='';
  NewFileName:='new.dic';
  ClearScreen;
  repeat
    AddScreen('');
    AddScreen('Source file      : '+SourceName);
    AddScreen('Destination file : '+NewFileName);
    AddScreen('');
    AddScreen('Characters...');
    AddScreen('Stressed letters  : '+EnabledDisabled(Authorized[0]));
    AddScreen('Punctuation signs : '+EnabledDisabled(Authorized[1]));
    AddScreen('Miscellaneous     : '+EnabledDisabled(Authorized[2]));
    AddScreen('Digits            : '+EnabledDisabled(Authorized[3]));
    AddScreen('');
    result:=Choice(Tab);
    case result of
      10:exit;
      11:AskScreen('> Source file : ',SourceName);
      12:AskScreen('> Destination file : ',NewFileName);
      13:MenuCharacter;
      14:ExtractWord(SourceName,NewFilename);
    end;
  until false;
end;

procedure MenuCount;
const
  Tab:array [0..4] of TChoice=
    ((name:'source';value:21;active:true),
     (name:'go!'   ;value:22;active:true),
     (name:''      ;value:23;active:false),
     (name:''      ;value:24;active:false),
     (name:'exit'  ;value:20;active:true));
var
  result:byte;
  SourceName:string;
begin
  SourceName:='';
  ClearScreen;
  repeat
    AddScreen('');
    AddScreen('Source file : '+SourceName);
    result:=Choice(Tab);
    case result of
      20:exit;
      21:AskScreen('> Source file : ',SourceName);
      22:CountWord(SourceName);
      23:exit;
      24:exit;
    end;
  until false;
end;

procedure MenuMix;
const
  Tab:array [0..4] of TChoice=
    ((name:'source#1'   ;value:31;active:true),
     (name:'source#2'   ;value:32;active:true),
     (name:'destination';value:33;active:true),
     (name:'go!'        ;value:34;active:true),
     (name:'exit'       ;value:30;active:true));
var
  result:byte;
  Source1Name,Source2Name,NewFileName:string;
begin
  Source1Name:='';
  Source2Name:='';
  NewFileName:='new.dic';
  ClearScreen;
  repeat
    AddScreen('');
    AddScreen('Source file 1    : '+Source1Name);
    AddScreen('Source file 2    : '+Source2Name);
    AddScreen('Destination file : '+NewFileName);
    result:=Choice(Tab);
    case result of
      30:exit;
      31:AskScreen('> Source file 1 : ',Source1Name);
      32:AskScreen('> Source file 2 : ',Source2Name);
      33:AskScreen('> Destination file : ',NewFileName);
      34:MixDic(Source1Name,Source2Name,NewFileName);
    end;
  until false;
end;

procedure MenuDivide;
const
  Tab:array [0..4] of TChoice=
    ((name:'source';value:41;active:true),
     (name:'size'  ;value:42;active:true),
     (name:'go!'   ;value:43;active:true),
     (name:''      ;value:44;active:false),
     (name:'exit'  ;value:40;active:true));
var
  result:byte;
  size:word;
  code:integer;
  SourceName,SizeString:string;
begin
  Size:=200;
  SizeString:=NewStr(Size);
  SourceName:='';
  ClearScreen;
  repeat
    AddScreen('');
    AddScreen('Source file : '+SourceName);
    AddScreen('Size        : '+SizeString);
    result:=Choice(Tab);
    case result of
      40:exit;
      41:AskScreen('> Source file : ',SourceName);
      42:begin
        AskScreen('> Size : ',SizeString);
        Size:=NewVal(SizeString,code);
        if code<>0 then begin
          AddScreen('Error! This is not a number!');
          Size:=200;
        end;
        SizeString:=NewStr(Size);
      end;
      43:DivideDic(SourceName,Size);
      44:exit;
    end;
  until false;
end;


procedure MainMenu;
const
  Tab:array [0..4] of TChoice=
    ((name:'extract';value:1;active:true),
     (name:'count'  ;value:2;active:true),
     (name:'mix'    ;value:3;active:true),
     (name:'divide' ;value:4;active:true),
     (name:'quit'   ;value:0;active:true));
var
  result:byte;
begin
  repeat
    ClearScreen;
    DisplayLogo;
    result:=Choice(Tab);
    case result of
      0:exit;
      1:MenuExtract;
      2:MenuCount;
      3:MenuMix;
      4:MenuDivide;
    end;
  until false;
end;




begin
  OffCursor;
  InitScreen;
  Authorized[0]:=true;
  Authorized[1]:=true;
  Authorized[2]:=true;
  Authorized[3]:=true;
  MainMenu;
end.
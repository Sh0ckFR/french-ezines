unit textscr;


interface

const
  ScreenWidth=74;
  ScreenHeigth=20;

var
  Screen:array [0..24] of string[79];


function Newstr(X:longint):string;
function NewVal(S:string;var Code:integer):longint;
function UpString(S:string):string;
function Blank(size:byte):string;
function Normalize(chaine:string):string;
function Centered(chaine:string):string;
procedure OffCursor;
procedure OnCursor;
procedure DisplayScreen;
procedure AddScreen(chaine:string);
procedure ScreenHeader(chaine:string);
procedure AskScreen(question:string;var answer:string);
procedure InitScreen;
procedure EmptyLines(number:byte);
procedure ClearScreen;


implementation

uses crt;

function NewStr(X:longint):string;
var S:string;
begin
  Str(X,S);
  Newstr:=S;
end;


function NewVal(S:string;var Code:integer):longint;
var R:integer;
begin
  Val(S,R,Code);
  Newval:=R;
end;


function UpString(S:string):string;
var j:word;
begin
  for j:=1 to length(S) do S[j]:=upcase(S[j]);
  UpString:=S;
end;


function Blank(size:byte):string;
var
  s:string;
  i:byte;
begin
  s:='';
  for i:=1 to size do s:=s+' ';
  blank:=s;
end;

function Normalize(chaine:string):string;
var
  S:string;
begin
  S:=chaine;
  if length(S)<ScreenWidth then S:=S+Blank(ScreenWidth-length(S))
  else if length (S)>ScreenWidth then S:=copy(S,1,ScreenWidth);
  Normalize:=S;
end;


function Centered(chaine:string):string;
var
  S:string;
begin
  S:=blank((ScreenWidth-length(chaine)) div 2)+chaine;
  Centered:=S;
end;




procedure OffCursor;assembler;
asm
  mov ah,01
  mov ch,15
  mov cl,0
  int $10
end;

procedure OnCursor;assembler;
asm
  mov ah,01
  mov ch,14
  mov cl,15
  int $10
end;



{DisplayScreen : procdure d'affichage de l'cran}

procedure DisplayScreen;
var
  i:byte;
begin
 clrscr;
 for i:=0 to 24 do Writeln(Screen[i]);
end;


{AddScreen : procedure d'affichage d'une nouvelle ligne … l'cran}

procedure AddScreen(chaine:string);
var
  i:byte;
begin
  for i:=5 to 21 do Screen[i-1]:=Screen[i];
  Screen[21]:=' บ '+Normalize(chaine)+' บ';
  DisplayScreen;
end;

{ScreenHeader : change l'entte de l'cran}

procedure ScreenHeader(chaine:string);
begin
  Screen[2]:=' บ '+Normalize(chaine)+' บ';
  DisplayScreen;
end;


{AskScreen : si on veut demander qqch … l'utilisateur}

procedure AskScreen(question:string;var answer:string);
begin
  Screen[23]:=' บ '+Normalize(question)+' บ';
  DisplayScreen;
  OnCursor;
  repeat
    gotoxy(length(question)+4,23);
    readln(answer);
  until answer<>'';
  OffCursor;
  Screen[23]:=' บ                                                                            บ';
end;

{InitScreen : procedure d'initialisatiopn de l'cran}

procedure InitScreen;
const
  empty=' บ                                                                            บ';
var
  i:byte;
begin
  Screen[1]:=' ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป';
  Screen[2]:=empty;
  Screen[3]:=' ฬออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออน';
  for i:=4 to 21 do Screen[i]:=empty;
  Screen[22]:=' ฬออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออน';
  Screen[23]:=empty;
  Screen[24]:=' ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ';
end;

{EmptyLines : ajoute des lignes vides}

procedure EmptyLines(number:byte);
var
  i:byte;
begin
  for i:=1 to Number do AddScreen('');
end;

procedure ClearScreen;
const
  empty=' บ                                                                            บ';
var
  i:byte;
begin
  for i:=4 to 21 do Screen[i]:=empty;
end;

end.
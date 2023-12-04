{ XOR-BASE64 Encrypt Filename against a String Key }

const
  KEY = 'My Secret Key'; // Set your key!
  DECODE = False; // "True" or "False"

function XorEnDeCrypt(const S: String): String;
var
  I, IK, Code: Integer;
begin
  IK := 1;
  Result := '';
  for I := 1 to Length(S) do
  begin
    if IK > Length(KEY) then IK := 1;
    Code := (Ord(S[I]) xor Ord(KEY[IK]));
    Result := Result + Chr(Code);
    IK := IK + 1;
  end;
end;

procedure ReplaceChar(var S: String; Find, Replace: Char);
var
  I: Integer;
begin
  for I:=1 to Length(S) do
    if S[I] = Find then
      S[I] := Replace;
end;

function Encrypt(const S: WideString): String;
begin
  Result := UTF8Encode(S);
  Result := XorEnDeCrypt(Result);
  Result := Base64Encode(Result);
  ReplaceChar(Result, '/', '=');
end;

function Decrypt(const S: String): WideString;
var
  Temp: String;
begin
  Temp := S;
  ReplaceChar(Temp, '=', '/');
  Temp := Base64Decode(Temp);
  Temp := XorEnDeCrypt(Temp);
  Result := UTF8Decode(Temp);
end;
  
begin
  if DECODE then
    FileName := Decrypt(FileName)
  else
    FileName := Encrypt(FileName);
end.
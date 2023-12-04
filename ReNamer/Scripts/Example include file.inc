// This file is used for demonstrating the use of {$INCLUDE} directive.

function ExampleFunction: String;
begin
  // Generate a random number seqeunce.
  Result := IntToStr(RandomRange(1000, 9999));
end;

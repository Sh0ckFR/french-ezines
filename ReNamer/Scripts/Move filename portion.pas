{ This script moves a portion of a filename }

const
  MOVE_START = 5;  // beginning of the movable portion
  MOVE_LENGTH = 3; // length of the movable portion
  MOVE_TO = 3;     // new position for the portion

var
  BaseName, Portion: WideString;

begin
  BaseName := WideExtractBaseName(FileName);
  Portion := WideCopy(BaseName, MOVE_START, MOVE_LENGTH);
  WideDelete(BaseName, MOVE_START, MOVE_LENGTH);
  WideInsert(Portion, BaseName, MOVE_TO);
  FileName := BaseName + WideExtractFileExt(FileName);
end.

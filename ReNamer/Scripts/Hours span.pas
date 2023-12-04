// This script reads dates from filenames in format: yyyy-mm-dd hh-nn-ss.JPG
// then adds/subtracts N hours from the date and prints the new date in the
// format defined by DateOutputFormat variable. HoursSpan variable defines
// how many hours should be added/subtracted (use minus for subtraction).

const
  HoursSpan = -3;   // amount of hours to add or subtract!!
  DateOutputFormat = 'yyyy-mm-dd hh.nn.ss';  // output date format!!
  HoursPerDay = 24;  // do not change this!!

var
  iYear, iMonth, iDay, iHour, iMin, iSec: Integer;
  Date, Time, DateTime: TDateTime;

procedure AddHours(var ADateTime: TDateTime; const ANumberOfHours: Integer);
begin
  ADateTime := ((ADateTime * HoursPerDay) + ANumberOfHours) / HoursPerDay;
end;

begin
  // extract date-time variables as integers
  iYear  := StrToIntDef(Copy(FileName, 1, 4), -1);
  iMonth := StrToIntDef(Copy(FileName, 6, 2), -1);
  iDay   := StrToIntDef(Copy(FileName, 9, 2), -1);
  iHour  := StrToIntDef(Copy(FileName, 12, 2), -1);
  iMin   := StrToIntDef(Copy(FileName, 15, 2), -1);
  iSec   := StrToIntDef(Copy(FileName, 18, 2), -1);

  // process only if all variables are correctly converted
  if (iYear >= 0) and (iMonth >= 0) and (iDay >= 0) and
     (iHour >= 0) and (iMin >= 0) and (iSec >= 0) then

  begin
    // create a new date-time variable
    Date := EncodeDate(iYear, iMonth, iDay);
    Time := EncodeTime(iHour, iMin, iSec, 0);
    DateTime := Date + Time;

    // add hours (use minus for subtracting)
    AddHours(DateTime, HoursSpan);

    // concatenate the rest of the filename and the new date
    FileName := Copy(FileName, 20, Length(FileName));
    FileName := FormatDateTime(DateOutputFormat, DateTime) + FileName;
  end

  // something went wrong
  else FileName := 'INVALID INPUT';
end.
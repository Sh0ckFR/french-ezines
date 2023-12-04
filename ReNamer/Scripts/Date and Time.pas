var
  DateTime: TDateTime;

begin
  DateTime := FileTimeModified(FilePath);
  FileName := WideExtractBaseName(FileName) +
    FormatDateTime(' (dd-mmm-yyyy)', DateTime) +
    WideExtractFileExt(FileName);
end.
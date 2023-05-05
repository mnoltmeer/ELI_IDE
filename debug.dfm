object DebugOutputForm: TDebugOutputForm
  Left = 0
  Top = 0
  Caption = 'Debug Output'
  ClientHeight = 475
  ClientWidth = 559
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  OnClose = FormClose
  PixelsPerInch = 96
  TextHeight = 13
  object DebugLog: TMemo
    Left = 0
    Top = 0
    Width = 559
    Height = 475
    Align = alClient
    ScrollBars = ssBoth
    TabOrder = 0
  end
end

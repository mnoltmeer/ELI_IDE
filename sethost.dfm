object SetHostAppForm: TSetHostAppForm
  Left = 0
  Top = 0
  BorderIcons = [biSystemMenu]
  BorderStyle = bsSingle
  Caption = 'Set Host Application'
  ClientHeight = 59
  ClientWidth = 545
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Position = poOwnerFormCenter
  PixelsPerInch = 96
  TextHeight = 13
  object Cancel: TBitBtn
    Left = 461
    Top = 31
    Width = 76
    Height = 25
    Kind = bkCancel
    NumGlyphs = 2
    TabOrder = 0
    OnClick = CancelClick
  end
  object Apply: TBitBtn
    Left = 380
    Top = 31
    Width = 76
    Height = 25
    Kind = bkOK
    NumGlyphs = 2
    TabOrder = 1
    OnClick = ApplyClick
  end
  object HostAppList: TComboBox
    Left = 2
    Top = 4
    Width = 489
    Height = 21
    TabOrder = 2
  end
  object SetHostApp: TButton
    Left = 497
    Top = 2
    Width = 40
    Height = 25
    Caption = '...'
    TabOrder = 3
    OnClick = SetHostAppClick
  end
  object OpenHostApp: TOpenDialog
    DefaultExt = 'exe'
    Filter = 'executable fles|*.exe'
    Left = 24
    Top = 8
  end
end

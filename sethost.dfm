object SetHostAppForm: TSetHostAppForm
  Left = 0
  Top = 0
  BorderIcons = [biSystemMenu]
  BorderStyle = bsSingle
  Caption = 'Set Host Application'
  ClientHeight = 75
  ClientWidth = 553
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  Position = poScreenCenter
  TextHeight = 13
  object Label1: TLabel
    Left = 8
    Top = 37
    Width = 253
    Height = 13
    Caption = 'Note: host application must accept argument list like:'
  end
  object Label2: TLabel
    Left = 8
    Top = 56
    Width = 306
    Height = 13
    Caption = '<ELI library path> <script path> <param1;param2...;paramn>'
  end
  object Cancel: TBitBtn
    Left = 467
    Top = 42
    Width = 76
    Height = 25
    Kind = bkCancel
    NumGlyphs = 2
    TabOrder = 0
    OnClick = CancelClick
  end
  object Apply: TBitBtn
    Left = 386
    Top = 42
    Width = 76
    Height = 25
    Kind = bkOK
    NumGlyphs = 2
    TabOrder = 1
    OnClick = ApplyClick
  end
  object HostAppList: TComboBox
    Left = 8
    Top = 8
    Width = 489
    Height = 21
    TabOrder = 2
  end
  object SetHostApp: TButton
    Left = 503
    Top = 6
    Width = 40
    Height = 25
    Caption = '...'
    TabOrder = 3
    OnClick = SetHostAppClick
  end
  object OpenHostApp: TOpenDialog
    DefaultExt = 'exe'
    Filter = 'executable fles|*.exe'
    Left = 368
    Top = 8
  end
end

object IDESettings: TIDESettings
  Left = 88
  Top = 93
  BorderIcons = [biSystemMenu]
  BorderStyle = bsSingle
  Caption = 'Settings'
  ClientHeight = 340
  ClientWidth = 417
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Position = poOwnerFormCenter
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Label2: TLabel
    Left = 8
    Top = 48
    Width = 62
    Height = 16
    Caption = 'Editor Font'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
  end
  object Label3: TLabel
    Left = 264
    Top = 48
    Width = 24
    Height = 16
    Caption = 'Size'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
  end
  object Label1: TLabel
    Left = 8
    Top = 8
    Width = 40
    Height = 16
    Caption = 'Theme'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
  end
  object Label4: TLabel
    Left = 8
    Top = 229
    Width = 68
    Height = 16
    Caption = 'ELI dll path:'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
  end
  object IsCorrectELIPath: TLabel
    Left = 96
    Top = 229
    Width = 110
    Height = 16
    Caption = 'IsCorrectELIPath'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'Tahoma'
    Font.Style = [fsBold, fsUnderline]
    ParentFont = False
  end
  object Label5: TLabel
    Left = 8
    Top = 191
    Width = 191
    Height = 16
    Caption = 'Count of Spaces for replace Tabs'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
  end
  object SaveSettings: TButton
    Left = 239
    Top = 301
    Width = 75
    Height = 25
    Caption = 'Save'
    TabOrder = 0
    OnClick = SaveSettingsClick
  end
  object CancelSettings: TButton
    Left = 328
    Top = 301
    Width = 80
    Height = 25
    Caption = 'Close'
    TabOrder = 1
    OnClick = CancelSettingsClick
  end
  object SelectTheme: TComboBox
    Left = 64
    Top = 7
    Width = 145
    Height = 21
    TabOrder = 2
    Text = 'Windows10'
    Items.Strings = (
      'Silver'
      'Windows10'
      'Windows10 SlateGray'
      'Onyx Blue')
  end
  object EnableSyntaxHighlight: TCheckBox
    Left = 8
    Top = 112
    Width = 198
    Height = 17
    Caption = 'Enable Syntax Highlight (F11)'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    TabOrder = 3
    OnClick = EnableSyntaxHighlightClick
  end
  object EnableCodeInsight: TCheckBox
    Left = 8
    Top = 135
    Width = 225
    Height = 17
    Caption = 'Enable Code Autocompletion (F12)'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    TabOrder = 4
    OnClick = EnableCodeInsightClick
  end
  object AutoShowCodeInsight: TCheckBox
    Left = 8
    Top = 158
    Width = 225
    Height = 17
    Caption = 'Auto show Autocompletion menu'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    TabOrder = 5
  end
  object SetELIPath: TButton
    Left = 383
    Top = 251
    Width = 25
    Height = 25
    Caption = '...'
    TabOrder = 6
    OnClick = SetELIPathClick
  end
  object ELIPath: TEdit
    Left = 8
    Top = 253
    Width = 369
    Height = 21
    TabOrder = 7
    Text = 'ELIPath'
    OnChange = ELIPathChange
  end
  object CountSpacesForTabs: TEdit
    Left = 224
    Top = 190
    Width = 49
    Height = 21
    TabOrder = 8
    Text = '2'
  end
  object SelectFont: TButton
    Left = 328
    Top = 70
    Width = 75
    Height = 25
    Caption = 'Select Font'
    TabOrder = 9
    OnClick = SelectFontClick
  end
  object CurrentFontSize: TEdit
    Left = 264
    Top = 70
    Width = 41
    Height = 24
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    TabOrder = 10
  end
  object CurrentFont: TEdit
    Left = 8
    Top = 70
    Width = 241
    Height = 24
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    ReadOnly = True
    TabOrder = 11
  end
  object OpenDLL: TOpenDialog
    DefaultExt = 'es'
    Filter = 'ELI library|ELI.dll'
    Left = 356
  end
  object FontDialog: TFontDialog
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
    MinFontSize = 8
    MaxFontSize = 20
    Options = [fdTrueTypeOnly, fdNoOEMFonts, fdLimitSize]
    Left = 296
  end
end

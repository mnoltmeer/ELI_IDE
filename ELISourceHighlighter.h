/*!
Copyright 2019-2020  Maxim Noltmeer (m.noltmeer@gmail.com)

This file is part of ELI IDE.

	ELI IDE is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	ELI IDE is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with ELI IDE.  If not, see <http://www.gnu.org/licenses/>.
*/
//---------------------------------------------------------------------------

#ifndef ELISourceHighlighterH
#define ELISourceHighlighterH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.ComCtrls.hpp>

#include <vector>

#define ESH_LIGHT_THEME 1
#define ESH_DARK_THEME 0

struct MarkedFragment
{
  int LineIndex;
  int SelPos;
  int SelLength;
  TColor SelColor;

  MarkedFragment(int line, int pos, int len, TColor color)
  {
	SelPos = pos;
    SelLength = len;
	SelColor = color;
  }
};

struct ExprColor
{
  wchar_t Expression[50];
  TColor Color;

  ExprColor(const wchar_t *expr, TColor color)
  {
	wcscpy(Expression, expr);
	Color = color;
  }
};

void InitExprColors(int theme_style);

String RemoveTabs(String fragment, int spaces_cnt); //������ ��� �� ������
int HighlightSource(TRichEdit *src, int line_ind); //������ ��������� � ��������� �����
int HighlightSourceFull(TRichEdit *src); //������ ��������� � ������ �����
void MarkFragmentsInLine(TRichEdit *src, int line_ind);
String CreateDummyString(int length);
void AddFunctionHighlight(const wchar_t *func_name, TColor highlight_color);
#endif

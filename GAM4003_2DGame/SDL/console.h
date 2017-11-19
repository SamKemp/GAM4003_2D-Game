/* ---------------------------------------------------------------
File: console.h
Author: Peter Lager
Version: 3.2
Date Last Modified: 5-02-2007

Version: 3.2.1
Casts to (SHORT) added to remove warnings. SM

Include to provide colours and cursor positioning for a win32
console application.

To use the features in this facility you need to #include this
file and add the following line
using namespace con;

COLOUR CONSTANTS
================
To simplify the use of colour in a Win32 console application
this file creates a number of colour constants. They are:-

Colour ID (normal)		Colour ID (dark)
Red				_red					_dk_red
Green			_green					_dk_green
Blue			_blue					_dk_blue
Cyan			_cyan					_dk_cyan
Magenta			_magenta				_dk_magenta
Yellow			_yellow					_dk_yellow
Grey			_grey					_dk_grey
White			_white
Black			_black

SETTING COLOURS AND CLEARING TEXT WITH COUT
===========================================
The following colours can be used as the foreground or
background colour for subsequent output to the console
window.

Prefix the colour constant ID with 'fg' for a foreground colour
and 'bg' for a background colour. For instance to display
'Hello World' with yellow text on a dark blue background use
the following

cout << fg_yellow << bg_dk_blue << "Hello World";

cout << clrscr;	    clears entire console buffer and moves the
cursor to the top left of the buffer
cout << clreol;     clears buffer to the end of the line from
the current cursor position.
cout << clrsol;		clears the buffer from the current cursor
position to the start of the line and the
cursor is moved to the start of the line.

*********************************************************************
*********************************************************************
CONSOLE MANIPULATION FUNCTIONS

The following text shows the function prototypes for a wide range of
console manipulation methods.
If a function has the following protoype
void myFunction();
then to call the function use
console.myFunction();
*********************************************************************

SETTING THE FOREGROUND AND BACKGROUND COLOUR ATTRIBUTES
=======================================================
A number of inline functions are provided to set the fore and
background colours.

void setFgColour( WORD fgColour );
void setBgColour( WORD bgColour );
void setColour(WORD fgColour, WORD bgColour);

where fgColour and bgColour are colour constant identifiers
e.g.
console.setColour(_yellow, _black);

It is also possible to work with colour attributes. A colour
attribute is of type WORD and holds bothe fore and backgorund
colour information.

void setColour(WORD col_attr);

To simplify the use of colour attributes the following function
creates and returns a colour attribute value given the fore and
background colours.

WORD makeAttr(WORD fgColour, WORD bgColour);

e.g.
WORD ca = console.makeAttr(_red, _white);
console.setColour(ca);
cout << clrscr;

Creats a colour attribute ca with a red foreground and white
background. Then uses this to set the console colour before
clearing the display to the new colours.

SAVING AND RESTORING CURRENT COLOUR ATTRIBUTES
==============================================
The following functions are used to save and restore text
attributes on a stack.

void save();
void restore();

When creating a function that changes the text attributes place
these statements at the start (save) and end (restore) of the
function body.

CLEARING AREAS OF THE SCREEN
============================
It is possible to clear some or all output in the console window
using the current foreground and background colour attributes.

void clear(wchar_t fill = ' ')     same effect as cout << clrscr;
void cleareol(wchar_t fill = ' '); same effect as cout << clreol;
void clearsol(wchar_t fill = ' '); same effect as cout << clrsol;

void clearwin(int x, int y, int width, int height,
wchar_t fill = ' ')
clears a rectangular portion of the screen

void clearbox(int x, int y, int width, int height,
wchar_t bdr, wchar_t fill = ' ')
same as previous function except that it draws
a box framed with the character bdr

In all of these functions the last parameter ('fill') is optional
and can be used to specify a fill character instead of the default
'space' character (ASCII code 32). The fill and bdr parameters can
be of type wchar_t (UNICODE) or char (ANSI).

The last 2 functions do not check that the values passed exceed
the bounds of the buffer width and height. Exceeding these values
will have unpredictable results.

POSITIONING THE CURSOR
======================
The cursor position can be set using one of the following
statements:

void setCurPosXY(int x, int y);
void setCurPosX(int x);			vertical position is unchanged
void setCurPosY(int y);			horizontal position is unchanged

where x and y represent the column and row within the screen
buffer respectively. Note that the top left position of the
buffer is 0, 0

The following 2 functions will return an integer representing the
horizontal and vertical positions of the current cursor position
respectively.

int getCurPosX();
int getCurPosY();

CONSOLE WINDOW SIZE
===================
int getWidth();       width of the console buffer
int getHeight();      height of the console buffer
int getWinWidth();    width of the console display window
int getWinHeight();   height of the console display window

int getMaxWidth();    maximum permitted width of the console display
window that will fit on the monitor
int getMaxHeight();   maximum permitted height of the console display
window that will fit on the monitor

CHANGING THE CONSOLE WINDOW AND BUFFER SIZE
===========================================
bool setSize(int w, int h, bool fixed = true);

Attempts to resize the console window display.

Both w and y must be >= 10.
If w > than the largest permissable width then the new width will
be set to the maximum size permitted.
If h > than the largest permissable height then the new height
will be set to the maximum size permitted.
If fixed = true then the screen buffer will be set to the same size
of the display window.
If fixed is false then the buffer size is not changed unless the
new display window is wider or higher than the current buffer in
which case the buffer's width and/or height is increaed to match
the display window.

bool setBufferSize(int w, int h);

Sets the console buffer size provided that it's width and height
are not smaller than the current console window's width and height.

Both of these methods return true if successful else returns false.

CHANGING TO FULL SCREEN MODE
============================
bool setDisplaySize(DWORD flag)
flag = FULLSCREEN (1) change to fullscreen mode
flag = NORMAL (2) change to normal console window mode

SETTING THE CONSOLE TITLE
=========================
To set the text console titlebar use any of the following

void setTitle(const wchar_t* title);
void setTitle(const char* title);
void setTitle(int anInt, char* format = "%d");
void setTitle(double aFloat, char* format = "%f");

FLICKER FREE DISPLAY METHODS
============================
Using cout to display text to a no visible area of the screen
buffer (not in the console window area) causes flickering.
These methods will enable flicker free display anywhere in the
screen buffer.
x and y indicates the column and row to start displaying the
value in the third parameter, The fouth parameter is optional
and clears nbrClear characters starting at x,y before displaying
the va;ue.

void print(int x, int y, double aFpn, int nbrClear = 0,
const char* format = "%f")
void print(int x, int y, int anInt, int nbrClear = 0,
const char* format = "%d");
void print(int x, int y, const char m[], int nbrClear = 0);
void print(int x, int y, string m, int nbrClear = 0, );

In the first method aFpn can also be of type float.

SCROLL METHODS
==============
The following methods allow you to scroll the display winow over
the screen buffer.

bool scrollUp(int d = 1);
bool scrollDown(int d = 1);
bool scrollLeft(int d = 1);
bool scrollRight(int d = 1);

The parameter d specifies the amount to scroll by.

bool scrollTo(int x, int y);	defines top-left corner for display

The functions return true if the display window can be scrolled
without exceeding the width / height restrictions of the screen
buufer else returnsfalse.

SETTING AND GETTING SINGLE CHARACTERS AND COLOUR ATTRIBUTES
===========================================================
ConCharPos getConCharPos(int x, int y);
ConChar getConChar(int x, int y);
TCHAR getChar(int x, int y);

bool setConCharPos(ConCharPos & cch);
bool setConChar(ConChar & cc, int x, int y);
bool setChar(char ch, int x, int y);

If x, y are outside the buffer then the value return by the
getters in indeterminate and the setters wil return false.

ConChar and ConCharPos are classes declared in this file and
provide container types to hold character, attribute and
positional data. See the class definitions below.

--------------------------------------------------------------- */

#ifndef __CONSOLE_H
#define __CONSOLE_H

//------------------------  INCLUDES ----------------------------
#include <windows.h>
#include <stack>	// Use default stack in STL
#include <string>
#include <string.h>

#pragma comment (lib, "kernel32.lib")

using namespace std;

namespace con
{
	/*
	This class provide a public access structure to hold
	a single character and its colour attributes.
	*/
	class ConChar
	{
	public:
		TCHAR ascii;
		WORD attr;

		ConChar()
		{
			ascii = '\0';
			attr = 0;
		}

		ConChar(TCHAR ch, WORD colour)
		{
			ascii = ch;
			attr = colour;
		}
	};

	/*
	This class provide a public access structure to hold
	a single character, the colour attribute and it's
	position in the screen buffer.
	*/
	class ConCharPos
	{
	public:
		TCHAR ascii;
		WORD attr;
		COORD pos;		// i.e. pos.X and pos.Y

		ConCharPos()
		{
			ascii = '\0';
			attr = 0;
			pos.X = pos.Y = 0;
		}

		ConCharPos(int x, int y)
		{
			ascii = '\0';
			attr = 0;
			pos.X = x;
			pos.Y = y;
		}

		ConCharPos(ConChar cc, int x, int y)
		{
			ascii = cc.ascii;
			attr = cc.attr;
			pos.X = x;
			pos.Y = y;
		}

		ConCharPos(COORD &p)
		{
			ascii = '\0';
			attr = 0;
			pos = p;
		}
	};

	// Create foreground and back ground colour masks
	static const WORD bgMask(BACKGROUND_BLUE |
		BACKGROUND_GREEN |
		BACKGROUND_RED |
		BACKGROUND_INTENSITY);
	static const WORD fgMask(FOREGROUND_BLUE |
		FOREGROUND_GREEN |
		FOREGROUND_RED |
		FOREGROUND_INTENSITY);

	// Create colour constants
	static const WORD _black(0);
	static const WORD _dk_red(FOREGROUND_RED);
	static const WORD _dk_green(FOREGROUND_GREEN);
	static const WORD _dk_blue(FOREGROUND_BLUE);
	static const WORD _dk_cyan(_dk_green | _dk_blue);
	static const WORD _dk_magenta(_dk_red | _dk_blue);
	static const WORD _dk_yellow(_dk_red | _dk_green);
	static const WORD _grey(_dk_red | _dk_green | _dk_blue);
	static const WORD _dk_grey(_black | FOREGROUND_INTENSITY);
	static const WORD _white(_grey | FOREGROUND_INTENSITY);
	static const WORD _blue(_dk_blue | FOREGROUND_INTENSITY);
	static const WORD _green(_dk_green | FOREGROUND_INTENSITY);
	static const WORD _red(_dk_red | FOREGROUND_INTENSITY);
	static const WORD _cyan(_dk_cyan | FOREGROUND_INTENSITY);
	static const WORD _magenta(_dk_magenta | FOREGROUND_INTENSITY);
	static const WORD _yellow(_dk_yellow | FOREGROUND_INTENSITY);

	static const DWORD FULLSCREEN = 1;
	static const DWORD NORMAL = 1;

	// ###########################################################
	// ################# CLASS HEADER ############################
	// ###########################################################
	/**
	This class provides a the means to manipulate the console window.

	To use the features in this facility you need to #include this
	file and add the following line
	\code using namespace console; \endcode

	When you do this a global object called 'console' will be available
	to call the methods.

	Full supporting comments are in the console.h file
	*/
	static class con_dev
	{
	protected:
		HANDLE						hOut;
		HANDLE						hIn;
		DWORD						cCharsWritten;
		CONSOLE_SCREEN_BUFFER_INFO	csbi;
		stack<WORD>					attrs;

		inline void getInfo()
		{
			GetConsoleScreenBufferInfo(hOut, &csbi);
		}

	public:
		con_dev()
		{
			hOut = GetStdHandle(STD_OUTPUT_HANDLE);
		}

		~con_dev()
		{
			while (!attrs.empty()) attrs.pop();
		}

	public:

		// ###########################################################
		// #           CONSOLE WINDOW TITLE METHODS                  #
		// ###########################################################
		void setTitle(const wchar_t* title)
		{
			SetConsoleTitle(title);
		}

		void setTitle(const char* title)
		{
			wchar_t wt[1024];
			wchar_t* pwt = wt;
			size_t nbr;

			// Parameter 3 = 1024 * sizeof(wchar_t) / sizeof(WORD)
			mbstowcs_s(&nbr, pwt, 1024, title, 1023);
			SetConsoleTitle(wt);
		}

		void setTitle(const int anInt, char* format = "%d")
		{
			char buffer[1024];
			sprintf_s(buffer, format, anInt);

			wchar_t wt[1024];
			wchar_t* pwt = wt;
			size_t nbr;

			// Parameter 3 = 1024 * sizeof(wchar_t) / sizeof(WORD)
			mbstowcs_s(&nbr, pwt, 1024, buffer, 1023);
			SetConsoleTitle(wt);
		}

		void setTitle(const double aFloat, char* format = "%f")
		{
			char buffer[1024];
			sprintf_s(buffer, format, aFloat);

			wchar_t wt[1024];
			wchar_t* pwt = wt;
			size_t nbr;

			// Parameter 3 = 1024 * sizeof(wchar_t) / sizeof(WORD)
			mbstowcs_s(&nbr, pwt, 1024, buffer, 1023);
			SetConsoleTitle(wt);
		}

		void setTitle(string title)
		{
			wchar_t wt[1024];
			wchar_t* pwt = wt;
			size_t nbr;
			const char * t = title.c_str();

			// Parameter 3 = 1024 * sizeof(wchar_t) / sizeof(WORD)
			mbstowcs_s(&nbr, pwt, 1024, t, 1023);
			SetConsoleTitle(wt);
		}

		// ###########################################################
		// #             CONSOLE BUFFER CLEARING METHODS             #
		// ###########################################################
		void clear(wchar_t fill = ' ')
		{
			COORD coordScreen = { 0, 0 };

			getInfo();
			FillConsoleOutputCharacter(hOut, fill,
				csbi.dwSize.X * csbi.dwSize.Y,
				coordScreen,
				&cCharsWritten);
			FillConsoleOutputAttribute(hOut,
				csbi.wAttributes,
				csbi.dwSize.X * csbi.dwSize.Y,
				coordScreen,
				&cCharsWritten);
			SetConsoleCursorPosition(hOut, coordScreen);
		}

		void clearwin(int x, int y, int width, int height,
			wchar_t fill = ' ')
		{
			getInfo();
			// get current cursor position
			COORD ccp = { csbi.dwCursorPosition.X, csbi.dwCursorPosition.Y };
			// Location top left of win
			COORD coordScreen = { (SHORT)x, (SHORT)y };

			for (int i = 0; i < height; i++)
			{
				coordScreen.Y = y + i;
				SetConsoleCursorPosition(hOut, coordScreen);

				FillConsoleOutputCharacter(hOut, fill,
					width,
					coordScreen,
					&cCharsWritten);
				FillConsoleOutputAttribute(hOut,
					csbi.wAttributes,
					width,
					coordScreen,
					&cCharsWritten);
			}
			SetConsoleCursorPosition(hOut, ccp);
		}

		void clearbox(int x, int y, int width, int height,
			wchar_t bdr, wchar_t fill = ' ')
		{
			getInfo();
			// get current cursor position
			COORD ccp = { csbi.dwCursorPosition.X, csbi.dwCursorPosition.Y };
			// Location top left of box
			COORD coordScreen = { (SHORT)x, (SHORT)y };

			for (int i = 0; i < height; i++)
			{
				coordScreen.X = x;
				coordScreen.Y = y + i;
				SetConsoleCursorPosition(hOut, coordScreen);
				// Top and bottom frame
				if (i == 0 || i == height - 1)
				{
					FillConsoleOutputCharacter(hOut, bdr,
						width,
						coordScreen,
						&cCharsWritten);
					FillConsoleOutputAttribute(hOut,
						csbi.wAttributes,
						width,
						coordScreen,
						&cCharsWritten);
				}
				else
				{
					FillConsoleOutputCharacter(hOut, fill,
						width,
						coordScreen,
						&cCharsWritten);
					// Blank line
					coordScreen.X = x;
					FillConsoleOutputAttribute(hOut,
						csbi.wAttributes,
						width,
						coordScreen,
						&cCharsWritten);
					// Left border
					coordScreen.X = x;
					FillConsoleOutputCharacter(hOut, bdr,
						1,
						coordScreen,
						&cCharsWritten);
					FillConsoleOutputAttribute(hOut,
						csbi.wAttributes,
						1,
						coordScreen,
						&cCharsWritten);
					// right border
					coordScreen.X = x + width - 1;
					FillConsoleOutputCharacter(hOut, bdr,
						1,
						coordScreen,
						&cCharsWritten);
					FillConsoleOutputAttribute(hOut,
						csbi.wAttributes,
						1,
						coordScreen,
						&cCharsWritten);
				}
			}
			SetConsoleCursorPosition(hOut, ccp);
		}

		void cleareol(wchar_t fill = ' ')
		{
			getInfo();
			COORD coordScreen = { csbi.dwCursorPosition.X,
				csbi.dwCursorPosition.Y };

			int length = csbi.dwSize.X - csbi.dwCursorPosition.X;

			FillConsoleOutputCharacter(hOut, fill,
				length,
				coordScreen,
				&cCharsWritten);
			FillConsoleOutputAttribute(hOut,
				csbi.wAttributes,
				length,
				coordScreen,
				&cCharsWritten);
			SetConsoleCursorPosition(hOut, coordScreen);
		}

		void clearsol(wchar_t fill = ' ')
		{
			getInfo();

			COORD coordScreen = { 0, csbi.dwCursorPosition.Y };

			int length = csbi.dwCursorPosition.X;

			FillConsoleOutputCharacter(hOut, fill,
				length,
				coordScreen,
				&cCharsWritten);
			FillConsoleOutputAttribute(hOut,
				csbi.wAttributes,
				length,
				coordScreen,
				&cCharsWritten);
			SetConsoleCursorPosition(hOut, coordScreen);
		}

		// ###########################################################
		// #        CONSOLE BUFFER SIZE RETRIEVE METHODS             #
		// ###########################################################
		int getWidth()
		{
			getInfo();
			return (int)csbi.dwSize.X;
		}

		int getHeight()
		{
			getInfo();
			return (int)csbi.dwSize.Y;
		}

		// ###########################################################
		// #        CONSOLE WINDOW SIZE RETRIEVE METHODS             #
		// ###########################################################
		int getWinMaxWidth()
		{
			getInfo();
			return (int)GetLargestConsoleWindowSize(hOut).X;
		}

		int getWinMaxHeight()
		{
			getInfo();
			return (int)GetLargestConsoleWindowSize(hOut).Y;
		}

		int GetWinWidth()
		{
			getInfo();
			return csbi.srWindow.Right - csbi.srWindow.Left + 1;
		}

		int GetWinHeight()
		{
			getInfo();
			return csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
		}

	protected:
		// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		// +                 SUPPORTING METHODS FOR                  +
		// +               WINDOW AND BUFFER RESIZING                +
		// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

		bool makeBigger(COORD s)
		{
			SMALL_RECT newRect;
			getInfo();
			if (SetConsoleScreenBufferSize(hOut, s) == 0)
				return false;
			newRect.Top = newRect.Left = 0;
			newRect.Right = s.X - 1;
			newRect.Bottom = s.Y - 1;
			if (SetConsoleWindowInfo(hOut, true, &newRect) == 0)
				return false;
			return true;
		}

		bool makeSmaller(COORD s)
		{
			SMALL_RECT newRect;
			getInfo();
			newRect.Top = newRect.Left = 0;
			newRect.Right = s.X - 1;
			newRect.Bottom = s.Y - 1;
			if (SetConsoleWindowInfo(hOut, true, &newRect) == 0)
				return false;
			if (SetConsoleScreenBufferSize(hOut, s) == 0)
				return false;
			return true;
		}

		bool setSizeFixed(int w, int h)
		{
			COORD newSize;
			bool ok = false;

			getInfo();
			if (h != csbi.dwSize.Y)
			{
				newSize = csbi.dwSize;
				newSize.Y = h;			// Change height first
				if (h < csbi.dwSize.Y)
					ok = makeSmaller(newSize);
				else
					ok = makeBigger(newSize);
				if (!ok) return false;
			}
			getInfo();
			if (w != csbi.dwSize.X)
			{
				newSize = csbi.dwSize;
				newSize.X = w;			// Change width
				if (w < csbi.dwSize.X)
					ok = makeSmaller(newSize);
				else
					ok = makeBigger(newSize);
				if (!ok) return false;
			}
			return true;
		}

		bool setSizeScrollable(int w, int h)
		{
			getInfo();

			// Initialise these values to the current buffer size
			// change these values to the new window size if need be.
			SHORT bufferWidthRequired = csbi.dwSize.X;
			SHORT bufferHeightRequired = csbi.dwSize.Y;
			bool changeBufferSize = false;

			// Increase width if neccessary
			if (w > (int)csbi.dwSize.X)
			{
				bufferWidthRequired = w;
				changeBufferSize = true;
			}
			// Increase height if neccessary
			if (h > (int)csbi.dwSize.Y)
			{
				bufferHeightRequired = h;
				changeBufferSize = true;
			}
			// Increase buffer as required
			if (changeBufferSize)
			{
				if (!setBufferSize(bufferWidthRequired, bufferHeightRequired))
					return false; // Unable to increase buffer
			}

			// Resize Window
			SMALL_RECT newRect;
			newRect.Top = newRect.Left = 0;
			newRect.Right = w - 1;
			newRect.Bottom = h - 1;
			if (SetConsoleWindowInfo(hOut, true, &newRect) == 0)
				return false; // Unable to resize window
			else
				return true;
		}

	public:
		// ###########################################################
		// #       SET WINDOW AND BUFFER SIZE METHODS                #
		// ###########################################################
		bool setSize(int w, int h, bool fixed = true)
		{
			getInfo();

			// Make sure that the new width and height are realistic values
			if (w < 10 || h < 10) return false;

			// Do not allow w or h to exceed maximum values
			COORD largest = GetLargestConsoleWindowSize(hOut);
			if (w >largest.X) w = largest.X;
			if (h > largest.Y) h = largest.Y;

			// Make sure there is a change in widow size
			if (csbi.dwSize.X == w && csbi.dwSize.Y == h)
				return false;

			// Fixed size console window with no scrolling
			// i.e. buffer size = window size
			if (fixed == true)
				return setSizeFixed(w, h);
			else
				return setSizeScrollable(w, h);
		}

		bool setBufferSize(int w, int h)
		{
			getInfo();
			COORD coord;
			coord.X = w;
			coord.Y = h;
			if (w >= csbi.srWindow.Right - csbi.srWindow.Left + 1 &&
				h >= csbi.srWindow.Bottom - csbi.srWindow.Top + 1)
			{
				SetConsoleScreenBufferSize(hOut, coord);
				return true;
			}
			else
				return false;
		}

		// ###########################################################
		// #             SCROLL METHODS                              #
		// ###########################################################
		bool scrollUp(int d = 1)
		{
			getInfo();
			if (csbi.srWindow.Top >= d)
			{
				SMALL_RECT srcWin;
				srcWin.Top = -d;
				srcWin.Bottom = -d;
				srcWin.Left = srcWin.Right = 0;
				SetConsoleWindowInfo(hOut, false, &srcWin);
				return true;
			}
			return false;
		}

		bool scrollDown(int d = 1)
		{
			getInfo();
			if (csbi.srWindow.Bottom < csbi.dwSize.Y - d)
			{
				SMALL_RECT srcWin;
				srcWin.Top = d;
				srcWin.Bottom = d;
				srcWin.Left = srcWin.Right = 0;
				return (SetConsoleWindowInfo(hOut, false, &srcWin) != 0);
			}
			return false;
		}

		bool scrollLeft(int d = 1)
		{
			getInfo();
			if (csbi.srWindow.Left >= d)
			{
				SMALL_RECT srcWin;
				srcWin.Left = -d;
				srcWin.Right = -d;
				srcWin.Top = srcWin.Bottom = 0;
				SetConsoleWindowInfo(hOut, false, &srcWin);
				return true;
			}
			return false;
		}

		bool scrollRight(int d = 1)
		{
			getInfo();
			if (csbi.srWindow.Right < csbi.dwSize.X - d)
			{
				SMALL_RECT srcWin;
				srcWin.Left = d;
				srcWin.Right = d;
				srcWin.Top = srcWin.Bottom = 0;
				SetConsoleWindowInfo(hOut, false, &srcWin);
				return true;
			}
			return false;
		}

		bool scrollTo(int x, int y)
		{
			getInfo();
			SMALL_RECT srcWin;
			srcWin.Left = x;
			srcWin.Right = x + csbi.srWindow.Right - csbi.srWindow.Left;
			srcWin.Top = y;
			srcWin.Bottom = y + csbi.srWindow.Bottom - csbi.srWindow.Top;
			if (x >= 0 && srcWin.Right <= csbi.dwSize.X
				&& y >= 0 && srcWin.Bottom <= csbi.dwSize.Y)
			{
				SetConsoleWindowInfo(hOut, true, &srcWin);
				return true;
			}
			return false;
		}

		// ###########################################################
		// #                  RETRIEVE METHODS                       #
		// ###########################################################
		ConCharPos getConCharPos(int x, int y)
		{
			TCHAR ch;
			WORD colour;
			DWORD nbrRead;			// dummy value for function call
			COORD loc = { (SHORT)x, (SHORT)y };
			ConCharPos ccp(loc);

			getInfo();
			if (x >= 0 && x < csbi.dwSize.X && y >= 0 && y < csbi.dwSize.Y)
			{
				ReadConsoleOutputCharacter(hOut, &ch, 1, loc, &nbrRead);
				ReadConsoleOutputAttribute(hOut, &colour, 1, loc, &nbrRead);
				ccp.ascii = ch;
				ccp.attr = colour;
			}
			return ccp;
		}

		ConChar getConChar(int x, int y)
		{
			TCHAR ch;
			WORD colour;
			DWORD nbrRead;			// dummy value for function call
			COORD loc = { (SHORT)x, (SHORT)y };
			ConChar cc;

			getInfo();
			if (x >= 0 && x < csbi.dwSize.X && y >= 0 && y < csbi.dwSize.Y)
			{
				ReadConsoleOutputCharacter(hOut, &ch, 1, loc, &nbrRead);
				ReadConsoleOutputAttribute(hOut, &colour, 1, loc, &nbrRead);
				cc.ascii = ch;
				cc.attr = colour;
			}
			return cc;
		}

		TCHAR getChar(int x, int y)
		{
			TCHAR ch;
			DWORD nbrRead;
			COORD loc = { (SHORT)x, (SHORT)y };

			getInfo();
			if (x >= 0 && x < csbi.dwSize.X && y >= 0 && y < csbi.dwSize.Y)
				ReadConsoleOutputCharacter(hOut, &ch, 1, loc, &nbrRead);
			return (char)ch;
		}

		// ###########################################################
		// #             SETTING CHARACTER DATA                      #
		// ###########################################################

		bool setConCharPos(ConCharPos & cch)
		{
			DWORD nbrWritten;
			getInfo();
			if (cch.pos.X >= 0 && cch.pos.X < csbi.dwSize.X
				&& cch.pos.Y >= 0 && cch.pos.Y < csbi.dwSize.Y)
			{
				FillConsoleOutputCharacter(hOut, cch.ascii, 1, cch.pos, &nbrWritten);
				FillConsoleOutputAttribute(hOut, cch.attr, 1, cch.pos, &nbrWritten);
				return true;
			}
			return false;
		}

		bool setConChar(ConChar & cc, int x, int y)
		{
			DWORD nbrWritten;
			COORD pos = { (SHORT)x, (SHORT)y };
			getInfo();
			if (x >= 0 && x < csbi.dwSize.X && y >= 0 && y < csbi.dwSize.Y)
			{
				FillConsoleOutputCharacter(hOut, cc.ascii, 1, pos, &nbrWritten);
				FillConsoleOutputAttribute(hOut, cc.attr, 1, pos, &nbrWritten);
				return true;
			}
			return false;
		}

		bool setChar(char ch, int x, int y)
		{
			DWORD nbrWritten;
			COORD pos = { (SHORT)x, (SHORT)y };
			getInfo();
			if (x >= 0 && x < csbi.dwSize.X && y >= 0 && y < csbi.dwSize.Y)
			{
				FillConsoleOutputCharacter(hOut, ch, 1, pos, &nbrWritten);
				FillConsoleOutputAttribute(hOut, csbi.wAttributes, 1, pos, &nbrWritten);
				return true;
			}
			return false;
		}

		// ###########################################################
		// #             ATTRIBUTE SAVE / RESTORE METHODS            #
		// ###########################################################
		void save()
		{
			getInfo();
			attrs.push(csbi.wAttributes);
		}

		void restore()
		{
			if (!attrs.empty())
			{
				csbi.wAttributes = attrs.top();
				attrs.pop();
				SetConsoleTextAttribute(hOut, csbi.wAttributes);
			}
		}

		// ###########################################################
		// #                 MAIN COLOURING METHODS                  #
		// ###########################################################
		inline void setColour(WORD fgColour, WORD bgColour)
		{
			getInfo();
			csbi.wAttributes = fgColour;
			csbi.wAttributes |= (bgColour << 4);
			SetConsoleTextAttribute(hOut, csbi.wAttributes);
		}

		inline void setColour(WORD col_attr)
		{
			getInfo();
			csbi.wAttributes = col_attr;
			SetConsoleTextAttribute(hOut, csbi.wAttributes);
		}

		inline void setFgColour(WORD fgColour)
		{
			getInfo();
			csbi.wAttributes &= bgMask;
			csbi.wAttributes |= fgColour;
			SetConsoleTextAttribute(hOut, csbi.wAttributes);
		}

		inline void setBgColour(WORD bgColour)
		{
			getInfo();
			csbi.wAttributes &= fgMask;
			csbi.wAttributes |= (bgColour << 4);
			SetConsoleTextAttribute(hOut, csbi.wAttributes);
		}

		inline WORD makeAttr(WORD fgColour, WORD bgColour)
		{
			WORD colour_attr = fgColour;
			colour_attr |= (bgColour << 4);
			return colour_attr;
		}

		// ###########################################################
		// #               CURSOR POSITIONING METHODS                #
		// ###########################################################
		int getCurPosX()
		{
			getInfo();
			return (int)csbi.dwCursorPosition.X;
		}

		int getCurPosY()
		{
			getInfo();
			return (int)csbi.dwCursorPosition.Y;
		}

		void setCurPosX(int x)
		{
			getInfo();
			COORD coordPosition = { (SHORT)x, (SHORT)csbi.dwCursorPosition.Y };
			SetConsoleCursorPosition(hOut, coordPosition);
		}

		void setCurPosY(int y)
		{
			getInfo();
			COORD coordPosition = { (SHORT)csbi.dwCursorPosition.X, (SHORT)y };
			SetConsoleCursorPosition(hOut, coordPosition);
		}

		void setCurPosXY(int x, int y)
		{
			getInfo();
			COORD coordPosition = { (SHORT)x, (SHORT)y };
			SetConsoleCursorPosition(hOut, coordPosition);
		}

		// ###########################################################
		// #           FLICKER FREE DISPLAY METHODS                  #
		// ###########################################################
		void print(int x, int y, int anInt, int nbrClear = 0,
			const char* format = "%d")
		{
			char buffer[1024];
			sprintf_s(buffer, format, anInt);
			print(x, y, buffer, nbrClear);
		}

		void print(int x, int y, double aFpn, int nbrClear = 0,
			const char* format = "%f")
		{
			char buffer[1024];
			sprintf_s(buffer, format, aFpn);
			print(x, y, buffer, nbrClear);
		}

		void print(int x, int y, const char m[], int nbrClear = 0)
		{
			getInfo();
			COORD coordScreen = { (SHORT)x, (SHORT)y };

			// Number of characters from x to the end of the line
			int length = csbi.dwSize.X - x;
			// If nbrClear is an acceptable value ie does not
			// extend bejond the end of the line then use it
			if (nbrClear >= 0 && nbrClear < length)
				length = nbrClear;
			// Clear the characters
			if (nbrClear != 0)
			{
				FillConsoleOutputCharacter(hOut, (wchar_t)' ',
					length,
					coordScreen,
					&cCharsWritten);
				FillConsoleOutputAttribute(hOut,
					csbi.wAttributes,
					length,
					coordScreen,
					&cCharsWritten);
			}
			// Convert char array to wchar_t array
			wchar_t wt[1024];
			wchar_t* pwt = wt;
			size_t nbr;
			// Parameter 3 = 1024 * sizeof(wchar_t) / sizeof(WORD)
			mbstowcs_s(&nbr, pwt, 1024, m, 1023);
			// Write output using current colour attributes
			WriteConsoleOutputCharacter(hOut, pwt, (DWORD)nbr, coordScreen, &cCharsWritten);
			FillConsoleOutputAttribute(hOut,
				csbi.wAttributes,
				(DWORD)nbr,
				coordScreen,
				&cCharsWritten);
		}

		void print(int x, int y, const string m, int nbrClear = 0)
		{
			getInfo();
			COORD coordScreen = { (SHORT)x, (SHORT)y };

			// Number of characters from x to the end of the line
			int length = csbi.dwSize.X - x;
			// If nbrClear is an acceptable value ie does not
			// extend bejond the end of the line then use it
			if (nbrClear >= 0 && nbrClear < length)
				length = nbrClear;
			// Clear the characters
			if (nbrClear != 0)
			{
				FillConsoleOutputCharacter(hOut, (wchar_t)' ',
					length,
					coordScreen,
					&cCharsWritten);
				FillConsoleOutputAttribute(hOut,
					csbi.wAttributes,
					length,
					coordScreen,
					&cCharsWritten);
			}
			// Convert char array to wchar_t array
			wchar_t wt[1024];
			wchar_t* pwt = wt;
			size_t nbr;
			const char * t = m.c_str();

			// Parameter 3 = 1024 * sizeof(wchar_t) / sizeof(WORD)
			mbstowcs_s(&nbr, pwt, 1024, t, 1023);
			// Write output using current colour attributes
			WriteConsoleOutputCharacter(hOut, pwt, (DWORD)nbr, coordScreen, &cCharsWritten);
			FillConsoleOutputAttribute(hOut,
				csbi.wAttributes,
				(DWORD)nbr,
				coordScreen,
				&cCharsWritten);
		}

		// ###########################################################
		// #                  CONSOLE SCREEN DISPLAY MODE            #
		// ###########################################################
		bool setDisplaySize(DWORD dwNewMode)
		{
			typedef BOOL(WINAPI *SCDMProc_t) (HANDLE, DWORD, LPDWORD);
			SCDMProc_t SetConsoleDisplayMode;
			HMODULE hKernel32;
			bool libraryLoaded = false, result;
			const char KERNEL32_NAME[] = "kernel32.dll";

			// Load Kernal32 library - exit function if unable
			// to load library
			hKernel32 = GetModuleHandleA(KERNEL32_NAME);
			if (hKernel32 == NULL)
			{
				hKernel32 = LoadLibraryA(KERNEL32_NAME);
				if (hKernel32 == NULL)
					return false;

				libraryLoaded = true;
			}

			// Library is open search for method SetConsoleDisplayMode
			// and get address into function pointer
			SetConsoleDisplayMode =
				(SCDMProc_t)GetProcAddress(hKernel32, "SetConsoleDisplayMode");

			if (SetConsoleDisplayMode == NULL)
			{
				SetLastError(ERROR_CALL_NOT_IMPLEMENTED);
				result = false;
			}
			else
			{
				DWORD dummy;
				result = (SetConsoleDisplayMode(hOut, dwNewMode, &dummy) != 0);
			}

			if (libraryLoaded)
				FreeLibrary(hKernel32);

			return result;
		} // end of setDisplaySize
	} console;

	// -----------------------------------------------------------------------
	// -----------------------------------------------------------------------

	// ###########################################################
	// #              CLEAR CONSOLE SCREEN METHODS               #
	// ###########################################################
	inline std::ostream& clrscr(std::ostream& os)
	{
		os.flush();
		console.clear();
		return os;
	}

	inline std::ostream& clreol(std::ostream& os)
	{
		os.flush();
		console.cleareol();
		return os;
	}

	inline std::ostream& clrsol(std::ostream& os)
	{
		os.flush();
		console.clearsol();
		return os;
	}

	// ###########################################################
	// #                  FOREGROUND COLOURING                   #
	// ###########################################################
	inline std::ostream& fg_red(std::ostream& os)
	{
		os.flush();
		console.setFgColour(_red);
		return os;
	}

	inline std::ostream& fg_dk_red(std::ostream& os)
	{
		os.flush();
		console.setFgColour(_dk_red);
		return os;
	}

	inline std::ostream& fg_green(std::ostream& os)
	{
		os.flush();
		console.setFgColour(_green);
		return os;
	}

	inline std::ostream& fg_dk_green(std::ostream& os)
	{
		os.flush();
		console.setFgColour(_dk_green);
		return os;
	}

	inline std::ostream& fg_blue(std::ostream& os)
	{
		os.flush();
		console.setFgColour(_blue);
		return os;
	}

	inline std::ostream& fg_dk_blue(std::ostream& os)
	{
		os.flush();
		console.setFgColour(_dk_blue);
		return os;
	}

	inline std::ostream& fg_white(std::ostream& os)
	{
		os.flush();
		console.setFgColour(_white);
		return os;
	}

	inline std::ostream& fg_cyan(std::ostream& os)
	{
		os.flush();
		console.setFgColour(_cyan);
		return os;
	}

	inline std::ostream& fg_dk_cyan(std::ostream& os)
	{
		os.flush();
		console.setFgColour(_dk_cyan);
		return os;
	}

	inline std::ostream& fg_magenta(std::ostream& os)
	{
		os.flush();
		console.setFgColour(_magenta);
		return os;
	}

	inline std::ostream& fg_dk_magenta(std::ostream& os)
	{
		os.flush();
		console.setFgColour(_dk_magenta);
		return os;
	}

	inline std::ostream& fg_yellow(std::ostream& os)
	{
		os.flush();
		console.setFgColour(_yellow);
		return os;
	}

	inline std::ostream& fg_dk_yellow(std::ostream& os)
	{
		os.flush();
		console.setFgColour(_dk_yellow);
		return os;
	}

	inline std::ostream& fg_black(std::ostream& os)
	{
		os.flush();
		console.setFgColour(_black);
		return os;
	}

	inline std::ostream& fg_grey(std::ostream& os)
	{
		os.flush();
		console.setFgColour(_grey);
		return os;
	}

	inline std::ostream& fg_dk_grey(std::ostream& os)
	{
		os.flush();
		console.setFgColour(_dk_grey);
		return os;
	}

	// ###########################################################
	// #                  BACKGROUND COLOURING                   #
	// ###########################################################
	inline std::ostream& bg_red(std::ostream& os)
	{
		os.flush();
		console.setBgColour(_red);
		return os;
	}

	inline std::ostream& bg_dk_red(std::ostream& os)
	{
		os.flush();
		console.setBgColour(_dk_red);
		return os;
	}

	inline std::ostream& bg_green(std::ostream& os)
	{
		os.flush();
		console.setBgColour(_green);
		return os;
	}

	inline std::ostream& bg_dk_green(std::ostream& os)
	{
		os.flush();
		console.setBgColour(_dk_green);
		return os;
	}

	inline std::ostream& bg_blue(std::ostream& os)
	{
		os.flush();
		console.setBgColour(_blue);
		return os;
	}

	inline std::ostream& bg_dk_blue(std::ostream& os)
	{
		os.flush();
		console.setBgColour(_dk_blue);
		return os;
	}

	inline std::ostream& bg_white(std::ostream& os)
	{
		os.flush();
		console.setBgColour(_white);
		return os;
	}

	inline std::ostream& bg_cyan(std::ostream& os)
	{
		os.flush();
		console.setBgColour(_cyan);
		return os;
	}

	inline std::ostream& bg_dk_cyan(std::ostream& os)
	{
		os.flush();
		console.setBgColour(_dk_cyan);
		return os;
	}

	inline std::ostream& bg_magenta(std::ostream& os)
	{
		os.flush();
		console.setBgColour(_magenta);
		return os;
	}

	inline std::ostream& bg_dk_magenta(std::ostream& os)
	{
		os.flush();
		console.setBgColour(_dk_magenta);
		return os;
	}

	inline std::ostream& bg_yellow(std::ostream& os)
	{
		os.flush();
		console.setBgColour(_yellow);
		return os;
	}

	inline std::ostream& bg_dk_yellow(std::ostream& os)
	{
		os.flush();
		console.setBgColour(_dk_yellow);
		return os;
	}

	inline std::ostream& bg_black(std::ostream& os)
	{
		os.flush();
		console.setBgColour(_black);
		return os;
	}

	inline std::ostream& bg_grey(std::ostream& os)
	{
		os.flush();
		console.setBgColour(_grey);
		return os;
	}

	inline std::ostream& bg_dk_grey(std::ostream& os)
	{
		os.flush();
		console.setBgColour(_dk_grey);
		return os;
	}
}

//------------------------------------------------------------------------------
#endif // #ifndef __CONSOLE_H

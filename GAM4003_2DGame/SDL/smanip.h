/*
File: smanip.h
Author: Peter Lager
Version: 2.2
Date Last Modified: 17-11-2007

Header file to provide addition functionality to work with
the STL string class (i.e. basic_string<char> )

To use the features in this facility you need to #include this
file and add the following line
using namespace strmanip;

MANIPULATION METHODS
ns = strman.lowercase(s);	ns = lowercase version of s
ns = strman.uppercase(s);	ns = uppercase vesrion of s
ns = strman.ltrim(s);		ns = copy of s without leading spaces
ns = strman.rtrim(s);		ns = copy of s without trailing spaces
ns = strman.trim(s);		ns = copy of s without leading or
trailing spaces

COMPARISON METHOD
icmp(s1, s2);		performs case insensitive comparison returns
true if they are the same else false

SERIALISATION METHODS
Two methods are provided to save strings to and load strings from
a file on disk. Both methods require a pointer to a valid FILE
structure (FILE is defined in stdio.h)
These methods will handle zero length (empty) strings without
any problem.
These functions provide no error handling.
strman.serialiseString(f, s);
s = strman.deserialise(f);
where
f = valid FILE pointer
s = string variable

KEYBOARD INPUT PARSING
int getCommand(vector<string> &cmd);
int getCommand(string cmd[]);
Both of these functions will accept input from the keyboard
until the <return> or <enter> key is pressed. Both functions
splits the input into 'words' using the space character as
the deliminator. The 'words' are placed in order into the
string array or vector depending on which version of the
function you are using. Both functions return an integer that
represents the number of 'words'
Usage
x = strman.getCommand(c);
where x is an integer and c is either an array or vector of
strings.
--------------------------------------------------------------- */

#ifndef __STRING_MANIP_H
#define __STRING_MANIP_H

#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>

using namespace std;

namespace strmanip
{
	/**
	This class provides additional functionality to manipulate objects
	of type 'string'.

	To use the features in this facility you need to #include this
	file and add the following line
	\code using namespace strmanip; \endcode

	When you do this a global object called 'strman' will be avaible
	to call the methods.

	Full supporting comments are in the smanip.h file
	*/
	static class StringManipulator
	{
	private:
		string tmp;
		int len, count;

	public:
		string lowercase(const string &s)
		{
			tmp = s;
			for (int i = 0; i < (int)tmp.length(); i++)
				if (tmp[i] >= 'A' && tmp[i] <= 'Z') tmp[i] |= 32;
			return tmp;
		}

		string uppercase(const string &s)
		{
			tmp = s;
			for (int i = 0; i < (int)tmp.length(); i++)
				if (tmp[i] >= 'a' && tmp[i] <= 'z') tmp[i] &= 223;
			return tmp;
		}

		string ltrim(const string &s)
		{
			tmp = s;
			len = (int)s.length();
			count = 0;

			while (count < len && s[count] == ' ')
				count++;

			if (count == len)
				tmp = "";
			else if (count != 0)
				tmp = s.substr(count);

			return tmp;
		}

		string rtrim(const string &s)
		{
			len = (int)s.length();
			count = len - 1;

			while (count >= 0 && s[count] == ' ')
				count--;

			if (count == 0)
				tmp = "";
			else if (count != len - 1)
				tmp = s.substr(0, count + 1);

			return tmp;
		}

		string trim(const string &s)
		{
			tmp = ltrim(s);
			tmp = rtrim(tmp);
			return tmp;
		}

		bool icmp(const string &s1, const string &s2)
		{
			return (lowercase(s1) == lowercase(s2));
		}

		void serialiseString(FILE* file, const string &s)
		{
			int len = (int)s.length();
			const char * c = s.c_str();
			fwrite(&len, sizeof(int), 1, file);
			fwrite(c, strlen(c), 1, file);
		}

		string deserialiseString(FILE* file)
		{
			int len = 0;
			fread(&len, sizeof(int), 1, file);
			char * c = new char[len + 1];
			fread(c, len, 1, file);
			c[len] = '\0';
			string s(c);
			delete[] c;
			return s;
		}

		int getCommand(vector<string> &cmd)
		{
			char next = '\0';
			string text;
			cmd.clear();

			while (next != '\n')
			{
				cin >> text;
				cmd.push_back(text);
				next = cin.peek();
			}
			// Remove EOL from input stream
			next = cin.get();
			return (int)cmd.size();
		}

		int getCommand(string cmd[])
		{
			char next = '\0';
			int count = 0;

			while (next != '\n')
			{
				cin >> cmd[count];
				count++;
				next = cin.peek();
			}
			// Remove EOL from input stream
			next = cin.get();
			return count;
		}
	} strman; // End of class
} // End of namespace
#endif;

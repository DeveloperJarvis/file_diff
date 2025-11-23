// --------------------------------------------------
// -*- C -*- Compatibility Header
//
// Copyright (C) 2023 Developer Jarvis (Pen Name)
//
// This file is part of the [Project Name] Library. This library is free
// software; you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the
// Free Software Foundation; either version 3, or (at your option)
// any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.
//
// SPDX-License-Identifier: GPL-3.0-or-later
//
// [Project Name] - [brief description of what it does]
//
// Author: Developer Jarvis (Pen Name)
// Contact: https://github.com/DeveloperJarvis
//
// --------------------------------------------------

// Header files
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// defines FOPEN for clang and other compilers
#if defined(_MSC_VER)
#define FOPEN(fptr, filepath, mode) fopen_s(fptr, filepath, mode)
#else
#define FOPEN(fptr, filepath, mode) (fptr = fopen(filepath, mode))
#endif

// other defines
#define MAX_LINE 2048
#define COLOR_RESET "\033[0m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"

// declarations for options
int opt_color = 0;		 // opt for color
int opt_char_diff = 0;	 // opt for character difference
int opt_unified = 0;	 // opt for unified
int opt_ignore_ws = 0;	 // opt for ignore whitespace
int opt_ignore_case = 0; // opt for ignore case

// ------------------------------------------------
// Forward function declarations
// ------------------------------------------------
void strip_whitespace(char *s);
void to_lowercase(char *s);
int compare_lines(const char *a, const char *b);
void char_diff(const char *a, const char *b);
void unified_print(int line, const char *a, const char *b);
void diff_files(const char *file1, const char *file2);
void display_help(char *arg);
void display_version(char *arg);
void parse_args(int argc, char *argv[], char **f1, char **f2);

// ------------------------------------------------
// Utility functions
// ------------------------------------------------

// Removes any white spaces
void strip_whitespace(char *s)
{
	char tmp[MAX_LINE];
	int idx = 0;
	for (int i = 0; s[i]; i++)
		if (!isspace((unsigned char)s[i]))
			tmp[idx++] = s[i];
	tmp[idx] = '\0';
	strcpy(s, tmp);
}

// Converts all text to lowercase
void to_lowercase(char *s)
{
	for (int i = 0; s[i]; i++)
		s[i] = tolower(s[i]);
}

// Compare with options enabled
int compare_lines(const char *a, const char *b)
{
	char la[MAX_LINE], lb[MAX_LINE];
	strcpy(la, a);
	strcpy(lb, b);

	if (opt_ignore_ws)
	{
		strip_whitespace(la);
		strip_whitespace(lb);
	}
	if (opt_ignore_case)
	{
		to_lowercase(la);
		to_lowercase(lb);
	}
	return strcmp(la, lb);
}

// ------------------------------------------------
// Character-by-character diff output
// ------------------------------------------------
void char_diff(const char *a, const char *b)
{
	int lenA = strlen(a);
	int lenB = strlen(b);
	int max = (lenA > lenB) ? lenA : lenB;

	printf("  ");

	for (int i = 0; i < max; i++)
	{
		if (i >= lenA)
		{
			printf("%s+%c%s", GREEN, b[i], COLOR_RESET);
		}
		else if (i >= lenB)
		{
			printf("%s-%c%s", RED, a[i], COLOR_RESET);
		}
		else if (a[i] != b[i])
		{
			printf("%s+%c%s", YELLOW, b[i], COLOR_RESET);
		}
		else
		{
			printf("%c", a[i]);
		}
	}
	printf("\n");
}

// ------------------------------------------------
// Unified diff (Git-style)
// ------------------------------------------------
void unified_print(int line, const char *a, const char *b)
{
	(void)line; // remove unused waring

	if (a)
	{
		printf("%s-%s%s", opt_color ? RED : "", a, opt_color ? COLOR_RESET : "");
	}
	if (b)
	{
		printf("%s-%s%s", opt_color ? GREEN : "", b, opt_color ? COLOR_RESET : "");
	}
}

// ------------------------------------------------
// MAIN diff engine
// ------------------------------------------------
void diff_files(const char *file1, const char *file2)
{
	FILE *fptr1;
	FOPEN(fptr1, file1, "r");
	FILE *fptr2;
	FOPEN(fptr2, file2, "r");
	if (fptr1 == NULL || fptr2 == NULL)
	{
		printf("Error opening files.\n");
		return;
	}

	char line1[MAX_LINE], line2[MAX_LINE];
	int line = 1;
	int differences = 0;

	while (1)
	{
		char *r1 = fgets(line1, MAX_LINE, fptr1);
		char *r2 = fgets(line2, MAX_LINE, fptr2);

		if (!r1 && !r2)
			break;

		if (!r1 || !r2 || compare_lines(line1, line2) != 0)
		{
			differences++;

			if (opt_unified)
			{
				printf("@@ -%d +%d @@\n", line, line);
				unified_print(line, r1 ? line1 : NULL, r2 ? line2 : NULL);
			}
			else
			{
				printf("Difference at line %d:\n", line);
				printf("File1: %s%s%s",
					   opt_color ? RED : "",
					   r1 ? line1 : "<EOF>\n",
					   opt_color ? COLOR_RESET : "");

				printf("File2: %s%s%s",
					   opt_color ? GREEN : "",
					   r2 ? line2 : "<EOF>\n",
					   opt_color ? COLOR_RESET : "");

				if (opt_char_diff && r1 && r2)
				{
					printf("Char diff:\n");
					char_diff(line1, line2);
				}

				printf("---------------------------------\n");
			}
		}
		line++;
	}

	if (differences == 0)
		printf("Files are identical.\n");
	else
		printf("Total differences %d\n", differences);

	fclose(fptr1);
	fclose(fptr2);
}

// ------------------------------------------------
// Parse command-line arguments
// ------------------------------------------------
void parse_args(int argc, char *argv[], char **f1, char **f2)
{
	if (argc < 3)
	{
		if (argc == 2 && (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0))
		{
			display_help(argv[0]);
			exit(1);
		}
		else if (argc == 2 && (strcmp(argv[1], "-v") == 0 || strcmp(argv[1], "--version") == 0))
		{
			display_version(argv[0]);
			exit(1);
		}
		else
		{
			printf("Invalid Usage.\n");
			display_help(argv[0]);
			exit(1);
		}
	}
	*f1 = argv[1];
	*f2 = argv[2];

	for (int i = 3; i < argc; i++)
	{
		if (strcmp(argv[i], "-c") == 0 || strcmp(argv[i], "--colored") == 0)
			opt_color = 1;
		if (strcmp(argv[i], "-cc") == 0 || strcmp(argv[i], "--charbychar") == 0)
			opt_char_diff = 1;
		if (strcmp(argv[i], "-i") == 0 || strcmp(argv[i], "--ignorecase") == 0)
			opt_ignore_case = 1;
		if (strcmp(argv[i], "-u") == 0 || strcmp(argv[i], "--unified") == 0)
			opt_unified = 1;
		if (strcmp(argv[i], "-w") == 0 || strcmp(argv[i], "--wsignore") == 0)
			opt_ignore_ws = 1;
	}
}

// ------------------------------------------------
// Display help and version
// ------------------------------------------------
// display help
void display_help(char *arg)
{
	printf("Usage: %s <file1> <file2> [options]\n", arg);
	printf("Options:\n");
	printf(" -c  --colored		colored difference\n");
	printf(" -cc --charbychar	character-by-character difference\n");
	printf(" -h  --help			display help\n");
	printf(" -i  --ignorecase	ignore case\n");
	printf(" -u  --unified		unified difference format\n");
	printf(" -v  --version		display version\n");
	printf(" -w  --wsignore		ignore whitespaces\n");
}

// display version
void display_version(char *arg)
{
	printf("%s version: 1.0.0\n", arg);
}

// ------------------------------------------------
// main()
// ------------------------------------------------
int main(int argc, char *argv[])
{
	char *file1, *file2;
	parse_args(argc, argv, &file1, &file2);

	diff_files(file1, file2);
	return 0;
}
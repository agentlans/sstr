#ifndef _SSTR
#define _SSTR

/*
	Copyright 2021 Alan Tseng

	Licensed under the Apache License, Version 2.0 (the "License");
	you may not use this file except in compliance with the License.
	You may obtain a copy of the License at

	    http://www.apache.org/licenses/LICENSE-2.0

	Unless required by applicable law or agreed to in writing, software
	distributed under the License is distributed on an "AS IS" BASIS,
	WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
	See the License for the specific language governing permissions and
	limitations under the License.
*/

#include <stdio.h>

/* File with an internal buffer to
  hold a line of text. */
struct sstr_file {
	FILE* file;
	char* line;
	size_t capacity;
};

/* Array of strings that come from splitting
 another string */
struct sstr_string_set {
	char** str;
	size_t size;
};

/* To open and close files */
struct sstr_file* sstr_open(const char* filename);
void sstr_close(struct sstr_file* obj);
/* Automatically allocated string containing a line from a file */
char* sstr_readline(struct sstr_file* obj);
int sstr_eof(struct sstr_file* obj);

/* For creating and manipulating heap-allocated strings */
char* sstr_str(const char* str);
int sstr_strcat(char** str1_p, const char* str2);
/* Truncates a string at its first newline character */
void sstr_chop_line(char* str);

/* For joining and splitting strings */
char* sstr_join(char** strings, size_t num_strings, const char* sep);
struct sstr_string_set* sstr_split(char* str, const char* delimiters);
void sstr_free_string_set(struct sstr_string_set* ss);

#endif

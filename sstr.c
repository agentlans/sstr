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

#include <stdlib.h>
#include <string.h>

#include "sstr.h"

void sstr_free_string_set(struct sstr_string_set* ss) {
	if (ss) free(ss->str);
	free(ss);
}

struct sstr_file* sstr_open(const char* filename) {
	struct sstr_file* obj = malloc(sizeof(struct sstr_file));
	if (!obj) return NULL;
	/* Open the file */
	obj->file = fopen(filename, "r");
	if (!obj->file) {
		free(obj);
		return NULL;
	}
	obj->line = NULL;
	obj->capacity = 0;
	return obj;
}

void sstr_close(struct sstr_file* obj) {
	fclose(obj->file);
	if (obj->line) free(obj->line);
	free(obj);
}

char* sstr_readline(struct sstr_file* obj) {
	int i = 0;
	/* Clear the line */
	memset(obj->line, 0, obj->capacity);
	while (!feof(obj->file)) {
		if (i == obj->capacity) {
			size_t new_capacity = obj->capacity + 100;
			/* If we can't allocate the memory, then close the file */
			char* p = realloc(obj->line, new_capacity);
			if (!p) {
				sstr_close(obj);
				return NULL;
			}
			obj->line = p;
			/* Set the new memory to 0 */
			memset(obj->line + obj->capacity, 0, 100);
			obj->capacity = new_capacity;
		}
		int c = fgetc(obj->file);
		/* End the line if any of these characters detected */
		if (c == '\n' || c == 0 || c == EOF) {
			obj->line[i] = 0;
			break;
		}
		obj->line[i] = c;
		i++;
	}
	return obj->line;
}

int sstr_eof(struct sstr_file* obj) {
	return feof(obj->file);
}

/* Returns heap-allocated null terminated string
with the same contents as the given string. */
char* sstr_str(const char* str) {
	char* s = malloc(strlen(str) + 1);
	if (!s) return NULL;
	strcpy(s, str);
	return s;
}

/*
str1_p: pointer to heap-allocated null terminated string
str2: null terminated string
*/
int sstr_strcat(char** str1_p, const char* str2) {
	size_t s1 = strlen(*str1_p);
	size_t s2 = strlen(str2);

	/* Set up enough space */
	char* p = realloc(*str1_p, s1 + s2 + 1);
	if (!p) {
		/* Couldn't set aside space. String unchanged. */
		return 1;
	}
	*str1_p = p;
	strcpy((*str1_p) + s1, str2);
}

char* sstr_join(char** strings, size_t num_strings, const char* sep) {
	/* Stores the lengths of strings */
	size_t strlens[num_strings];
	size_t sep_len = strlen(sep);

	size_t total_len = 1; /* At least one character for null terminator */
	for (int i = 0; i < num_strings; ++i) {
		strlens[i] = strlen(strings[i]);
		total_len += strlens[i];
	}
	total_len += sep_len * (num_strings - 1);
	/* Allocate space for the joined string */
	char* out = malloc(total_len);
	if (!out) return NULL;
	
	int ns = num_strings - 1;
	int place = 0;
	for (int i = 0; i < ns; ++i) {
		/* Copy the string */
		memcpy(out + place, strings[i], strlens[i]);
		place += strlens[i];
		/* Copy the separator */
		memcpy(out + place, sep, sep_len);
		place += sep_len;
	}
	/* Copy the last string */
	memcpy(out + place, strings[ns], strlens[ns]);
	out[total_len - 1] = 0;
	return out;
}

/* Discards every character after and including the first newline */
void sstr_chop_line(char* str) {
	char* nl = strchr(str, '\n');
	if (nl) *nl = 0;
}

/* Splits str into tokens in place.
   Returns array of pointers to the splitted strings.
   The number of tokens is stored in len. */
struct sstr_string_set* sstr_split(char* str, const char* delimiters) {
	/* Declare new string set on heap */
	struct sstr_string_set* ss = malloc(sizeof(struct sstr_string_set));
	if (!ss) return NULL;
	/* Initialize the string set */
	ss->str = NULL;
	ss->size = 0;
	int capa = 0;
	/* Keep calling strtok until no more string. */
	char* p = strtok(str, delimiters);
	int i = 0;
	while (p != NULL) {
		if (i >= capa - 1) {
			/* Expand the array */
			char** ptr = realloc(ss->str, sizeof(char*) * (capa + 10));
			if (!ptr) {
				sstr_free_string_set(ss);
				return NULL;
			}
			ss->str = ptr;
			capa += 10;
		}
		/* Save the string */
		ss->str[i] = p;
		p = strtok(NULL, delimiters);
		i++;
	}
	ss->size = i;
	ss->str[i + 1] = NULL;
	return ss;
}

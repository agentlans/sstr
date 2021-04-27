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
#include <stdlib.h>

#include "sstr.h"

int main() {
	/* Open a file, read each line, and print it out on screen. */
	struct sstr_file* f = sstr_open("Ozymandias.txt");
	while (!sstr_eof(f)) {
		/* Note: do not call free() on the line. sstr_close will do that automatically. */
		char* line = sstr_readline(f);
		printf("%s\n", line);
	}
	sstr_close(f);

	/* String concatenation example */
	char* s = sstr_str("Hello");
	sstr_strcat(&s, " world");
	printf("%s\n", s);
	free(s);

	/* Joining array of strings */
	char* strs[] = {"asdf", "zxb", "item3"};
	char* s2 = sstr_join(strs, 3, ",");
	printf("%s\n", s2);
	free(s2);

	/* Splitting a string */
	char s3[] = "foo|bar|baz";
	struct sstr_string_set* ss = sstr_split(s3, "|");
	for (int i = 0; i < ss->size; ++i) {
		printf("%s\n", ss->str[i]);
	}
	/* Deallocates the string set but not the string pointers inside it */
	sstr_free_string_set(ss);
	
	return 0;
}

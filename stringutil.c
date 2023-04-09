#include "stringutil.h"

int last_line_length(const char* str, size_t str_len) {
    int len = 0;
    for (int x = str_len -1; x >= 0; --x) {
        if (str[x] == '\n') break; 
        len++;
    }

    return len;
}

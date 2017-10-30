#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static char* dupAndQuote(const char *s);
int pm_command(char * serial, int argc, char ** argv);

void main(int argc, char** argv)
{
	char *serial = argv[1];
	pm_command(serial, argc, argv);
}

/** duplicate string and quote all \ " ( ) chars + space character. */
static char *dupAndQuote(const char *s)
{
    const char *ts;
    size_t alloc_len;
    char *ret;
    char *dest;
    ts = s;
    alloc_len = 0;
    for( ;*ts != '\0'; ts++) {
        alloc_len++;
        if (*ts == ' ' || *ts == '"' || *ts == '\\' || *ts == '(' || *ts == ')') {
            alloc_len++;
        }
    }
    ret = (char *)malloc(alloc_len + 1);
    ts = s;
    dest = ret;
    for ( ;*ts != '\0'; ts++) {
        if (*ts == ' ' || *ts == '"' || *ts == '\\' || *ts == '(' || *ts == ')') {
            *dest++ = '\\';
        }
        *dest++ = *ts;
    }
    *dest++ = '\0';
    return ret;
}

int pm_command(char* serial, int argc, char** argv)
{
    char buf[4096];
	int tmp = argc;
    snprintf(buf, sizeof(buf), "shell:pm");
    while(tmp-- > 0) {
        char *quoted;
        quoted = dupAndQuote(*argv++);
        strncat(buf, " ", sizeof(buf)-1);
        strncat(buf, quoted, sizeof(buf)-1);
        free(quoted);
    }
    printf("test[%s][%d][%s][%s]\n", argv[tmp], argc, serial, buf);
    return 0;
}


#include "shell.h"

/* history storage */
static char* history_arr[HISTORY_SIZE];
static int history_count = 0;   /* total commands stored (<= HISTORY_SIZE) */
static int history_next = 0;    /* index to overwrite next (0..HISTORY_SIZE-1) */

void add_history_entry(const char* cmd) {
    if (cmd == NULL) return;
    /* make a copy */
    char* copy = strdup(cmd);
    if (!copy) return;
    /* if existing cell occupied, free it */
    if (history_arr[history_next] != NULL) free(history_arr[history_next]);
    history_arr[history_next] = copy;
    history_next = (history_next + 1) % HISTORY_SIZE;
    if (history_count < HISTORY_SIZE) history_count++;
}

void print_history() {
    int start = (history_count == HISTORY_SIZE) ? history_next : 0;
    for (int i = 0; i < history_count; i++) {
        int idx = (start + i) % HISTORY_SIZE;
        printf("%d %s\n", i + 1, history_arr[idx]);
    }
}

char* get_history_entry(int n) {
    /* n is 1-based index as shown by print_history */
    if (n < 1 || n > history_count) return NULL;
    int start = (history_count == HISTORY_SIZE) ? history_next : 0;
    int idx = (start + (n - 1)) % HISTORY_SIZE;
    return history_arr[idx]; /* do not free here */
}

char* read_cmd(char* prompt, FILE* fp) {
    printf("%s", prompt);
    char* cmdline = (char*) malloc(sizeof(char) * MAX_LEN);
    int c, pos = 0;

    while ((c = getc(fp)) != EOF) {
        if (c == '\n') break;
        cmdline[pos++] = c;
    }

    if (c == EOF && pos == 0) {
        free(cmdline);
        return NULL; // Handle Ctrl+D
    }
    
    cmdline[pos] = '\0';
    return cmdline;
}

char** tokenize(char* cmdline) {
    // Edge case: empty command line
    if (cmdline == NULL || cmdline[0] == '\0' || cmdline[0] == '\n') {
        return NULL;
    }

    char** arglist = (char**)malloc(sizeof(char*) * (MAXARGS + 1));
    for (int i = 0; i < MAXARGS + 1; i++) {
        arglist[i] = (char*)malloc(sizeof(char) * ARGLEN);
        bzero(arglist[i], ARGLEN);
    }

    char* cp = cmdline;
    char* start;
    int len;
    int argnum = 0;

    while (*cp != '\0' && argnum < MAXARGS) {
        while (*cp == ' ' || *cp == '\t') cp++; // Skip leading whitespace
        
        if (*cp == '\0') break; // Line was only whitespace

        start = cp;
        len = 1;
        while (*++cp != '\0' && !(*cp == ' ' || *cp == '\t')) {
            len++;
        }
        strncpy(arglist[argnum], start, len);
        arglist[argnum][len] = '\0';
        argnum++;
    }

    if (argnum == 0) { // No arguments were parsed
        for(int i = 0; i < MAXARGS + 1; i++) free(arglist[i]);
        free(arglist);
        return NULL;
    }

    arglist[argnum] = NULL;
    return arglist;
}

/* Handle built-in commands: return 1 if handled, 0 otherwise */
int handle_builtin(char** arglist) {
    if (arglist == NULL || arglist[0] == NULL) return 0;

    if (strcmp(arglist[0], "exit") == 0) {
        /* free resources if needed, then exit */
        exit(0);
    }

    if (strcmp(arglist[0], "cd") == 0) {
        if (arglist[1] == NULL) {
            fprintf(stderr, "cd: missing argument\n");
        } else {
            if (chdir(arglist[1]) != 0) {
                perror("cd");
            }
        }
        return 1;
    }

    if (strcmp(arglist[0], "help") == 0) {
        printf("Built-in commands:\n");
        printf("  exit        Exit the shell\n");
        printf("  cd <dir>    Change directory\n");
        printf("  help        Show this help\n");
        printf("  jobs        Job control (not yet implemented)\n");
        return 1;
    }

    if (strcmp(arglist[0], "jobs") == 0) {
        printf("Job control not yet implemented.\n");
        return 1;
    }

    return 0;
}

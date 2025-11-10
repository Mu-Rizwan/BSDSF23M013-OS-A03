#include "shell.h"
#include <ctype.h>

int main() {
    char* cmdline;
    char** arglist;

    while ((cmdline = read_cmd(PROMPT, stdin)) != NULL) {
    	/* trim newline if present */
    	if (cmdline[0] == '\0') { free(cmdline); continue; }

    	/* Handle !n: re-execute from history */
    	if (cmdline[0] == '!' && isdigit((unsigned char)cmdline[1])) {
        	int n = atoi(&cmdline[1]);
        	char* old = get_history_entry(n);
        	if (old == NULL) {
            		printf("No such history entry %d\n", n);
            		free(cmdline);
            		continue;
        	}
        	else {
            		/* replace cmdline with the historical command */
            		free(cmdline);
            		cmdline = strdup(old);
            		if (!cmdline) break; /* out of memory, exit */
            		printf("%s\n", cmdline); /* echo the command we will run */
        	}
    	}

    	if ((arglist = tokenize(cmdline)) != NULL) {
        	/* add to history only if tokenized non-empty */
        	add_history_entry(cmdline);

        	if (strcmp(arglist[0], "history") == 0) {
            		print_history();
        	} else if (!handle_builtin(arglist)) {
            		execute(arglist);
        	}

        	for (int i = 0; arglist[i] != NULL; i++) free(arglist[i]);
        	free(arglist);
    	}
    	free(cmdline);
}

    printf("\nShell exited.\n");
    return 0;
}

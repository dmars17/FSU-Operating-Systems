#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAXCMD 64 				// Maximum commands allowed
#define MAXCMDLEN 512 		// Maximum command line length
#define MAXARG 256				// Maximum amount of arguments
#define MAXLINE 4094			// Maxumum line length
#define MAXLINEPLUS 4096 	// Maximum line lenght with null

/* Global Variables and Data Structures */
static char *cmd_sep = ";";
static char *delim = " \t\n";
static char *ent = "\r";

int exit_flag = 0;

char *com[MAXARG];
char line[MAXLINEPLUS];
char *cmds[MAXCMD];

/* Helper Functions */
void
reset_com(){
  memset(com, 0, MAXARG);
}

void
reset_cmds(){
  memset(cmds, 0, MAXCMD);
}

void
reset_line(){
  memset(line, 0, MAXLINEPLUS);
}

void
dump_com(){
  int i;
  for (i = 0; com[i]; ++i){
    printf(" com[%d] = %s\n", i, com[i]);
  }
}

void
dump_cmds(){
  int i;
  for (i = 0; cmds[i]; ++i){
    printf(" cmds[%d] = %s\n", i, cmds[i]);
  }
}


/******************/
/*  Main Program  */
/******************/

int
main(int argc, char *argv[MAXCMD]){

  /* Clear out the buffers */
  reset_com();
  reset_cmds();
  reset_line();

  int i = 0;				// Control variable
  int j = 0;				// Control variable

  /* Parsing variables */
  char *token = NULL;
  char *ptr = NULL;
  char *endptr = NULL;

  /* Demo Loop */
  while (fgets(line, sizeof(line), stdin))
  {
    if (strlen(line) > MAXLINE){
      perror("Input line length exceeded. Retry.\n");
    }

    if (isspace(line[0]) || line[0] == 0 || line[0] == 13){
      continue;
    } 






    i = 0;
    token = strtok(line, cmd_sep);
    if (strcmp(token, "quit\n") == 0) exit_flag = 1;
    while (token != NULL){
      if (strcmp(token, " ") != 0){
				cmds[i] = token;
				printf("cmds[%d]: %s\n", i, cmds[i]);
				++i;
      }
			token = strtok(NULL, cmd_sep);
    } // end while

		cmds[i] = NULL;

		for (i = 0; cmds[i]; ++i){
			j=0;
			printf("\nProcessing command '%s' ...\n", cmds[i]);

			// check command length
			if (strlen(cmds[i]) > MAXCMDLEN)	{
				perror("Maximum command length exceeded. Ignoring command.");
				continue;
			}

			token = strtok(cmds[i], delim);
			if (strcmp(token, "quit") == 0) exit_flag = 1;
			while (token != NULL) {
				com[j++] = token;
				token = strtok(NULL, delim);
			} // end while

			com[j] = NULL;
			
			printf("Parsed command = \n");
			dump_com();

			printf("Executing command ... \n");
      reset_com();
			printf("Done \n\n");

		} //end for

		if(exit_flag) break;
		reset_line();
		reset_cmds();

  } // end while

	if (exit_flag) exit(EXIT_SUCCESS);

	return 0;


}

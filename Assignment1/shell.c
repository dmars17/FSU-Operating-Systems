/* ************************ */
/* Dustin Marsden						*/
/* DM18CK										*/
/* Last update 9/10/2021		*/
/* References: sample code	*/
/* from  Dr. Uh and Derek		*/
/* Yohn.										*/
/* ************************	*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define MAXCMD 64 				// Maximum commands allowed
#define MAXCMDLEN 512 		// Maximum command line length
#define MAXARG 256				// Maximum amount of arguments
#define MAXLINE 4094			// Maxumum line length
#define MAXLINEPLUS 4096 	// Maximum line lenght with null

/* Global Variables and Data Structures */
static char *cmd_sep = ";";
static char *delim = " \t\n";

int exit_flag = 0;
int batch = 0;

char *com[MAXARG];
char line[MAXLINEPLUS];
char *cmds[MAXCMD];

/* Helper Functions */
void
reset_com(){									/* resets com array */
  memset(com, 0, MAXARG);
}

void
reset_cmds(){									/* resets cmds array */
  memset(cmds, 0, MAXCMD);
}

void
reset_line(){									/* resets line array */
  memset(line, 0, MAXLINEPLUS);
}

void
dump_com(){										/* outputs com array */
  int i;
  for (i = 0; com[i]; ++i){
    printf(" com[%d] = %s\n", i, com[i]);
  }
}

void
dump_cmds(){									/* outputs cmds array */
  int i;
  for (i = 0; cmds[i]; ++i){
    printf(" cmds[%d] = %s\n", i, cmds[i]);
  }
}

void execute(char **argv)     /* function to exeuce the fork and execvp */         
{
     pid_t pid;    
     int status;                 
     
     if ((pid = fork()) < 0) {     										/* fork a child process */     
          printf("*** ERROR: forking child process failed\n");
          exit(1);
     }
     else if (pid == 0) {          										/* for the child process */
          if (execvp(*argv, argv) < 0) {     					/* execute the command  */
               printf("*** ERROR: exec failed\n");
               exit(1);
          }
     }
     else {                                   		/* for the parent */
          while (wait(&status) != pid);       		/* wait for completion */
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
 
	if (argc > 1){						/* checks for more than 1 argument meaning a file is being uploaded */
		freopen(argv[1], "r", stdin);
		if (stdin == NULL){
			perror("Batch file error.\n");
			exit(EXIT_FAILURE);
		}
		batch = 1;
	}

	/* if no file is loaded start a command prompt */

  if(!batch) printf("Shell-> ");

  /* Demo Loop */
  while (fgets(line, sizeof(line), stdin))									/* reads in a line */
  {
    if (strlen(line) > MAXLINE){
      perror("Input line length exceeded. Retry.\n");
    }

    if (isspace(line[0]) || line[0] == 0 || line[0] == 13){
      continue;
    } 

    i = 0;
    token = strtok(line, cmd_sep);											/* starts creating tokens to store into arrays */
    if (strcmp(token, "quit\n") == 0) exit_flag = 1;		/* if quit is found set the exit flag to continue running program */
    while (token != NULL){
      if (strcmp(token, " ") != 0){
				cmds[i] = token;																/* loads tokens into cmds array */
				printf("cmds[%d]: %s\n", i, cmds[i]);
				++i;
      }
			token = strtok(NULL, cmd_sep);
    } // end while

		cmds[i] = NULL;																			/* sets last of cmds array to NULL */



		for (i = 0; cmds[i]; ++i){													/* second round of parsing */
			j=0;
			printf("\nProcessing command '%s' ...\n", cmds[i]);

			// check command length
			if (strlen(cmds[i]) > MAXCMDLEN)	{
				perror("Maximum command length exceeded. Ignoring command.");
				continue;
			}

			token = strtok(cmds[i], delim);									/* uses white space as delimiters */
			if (strcmp(token, "quit") == 0) exit_flag = 1;
			while (token != NULL) {
				com[j++] = token;															/* stores new tokens into coms array */
				token = strtok(NULL, delim);
			} // end while

			com[j] = NULL;
			
			printf("Parsed command = \n");
			dump_com();

			printf("Executing command ... \n");
			
			execute(com);																		/* calls the execute function */

      reset_com();																		/* clears out com array */
			printf("Done \n\n");

		} //end for

		if(exit_flag) break;															/* if exit flag was set exit the loop  */
		reset_line();																			/* clears out line array */
		reset_cmds();																			/* clears out cmds array */
		
		printf("Shell -> ");															/* prints prompt again */
  } // end while

	
	if (exit_flag) exit(EXIT_SUCCESS);									/* exit program */

	return 0;


}

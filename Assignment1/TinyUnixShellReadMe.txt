Dustin Marsden
DM18CK

	This code will run a command line interpreter. It will operate as follows. It will read a line. Then
it will parse the code into usable commands to be executed by the program. After the program has usable
commands it will fork a child process to concurrently run multiple commands. Once the commands have 
finished, the program will reprompt the user to input more commands unless the quit command was entered.
The program will also work two different ways, interactive and batch. Interactive is the user inputing 
commands while the batch will use a text file with commands.

	The code has 3 main parts. First is the parsing loops. These loops break the code into usable commands.
It uses two loops for this. It reads in a line then stores the line in the line array. Once in the line
array, it creates tokens seperated by a ";". It stores these tokens into the cmds array. This is the
array that holds the whole command eg. ls -al. Once this array has been filled it moves to the second 
parsing. This time it removes white space and breaks down the commands to individual parts. These are
stored in the com array. Using ls - al, it would break down to com[0] = ls and com[1] = -al. If any of
these parses find the "quit" command it will set an exit flag to use for exiting later. With this flag, 
it guarentees the program will run until all commands are executed.

	Second main part is the batch check. It sets a bool batch to 0. When the program is called, a single
executable means the program will run in interactive mode. However, if the program is called with
another command, it means that there is a text file looking to be uploaded. The program will set batch
to 1, skip the prompt and begin executing the commands.

	The third main part is the execute function. This function takes in the com array. It starts fork()
to create child processes to execute each command concurrently. 	

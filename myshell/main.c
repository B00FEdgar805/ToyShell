/*
 Edgar Alamillo
 Created: 09/20/2025
 This is a toy shell written in c
 */

#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

//Global variables

char *SHELL_NAME = "myshell";
char *TERMINATOR = ">";
int NUM_OF_NEW_NAMES = 0;

//New name arrays

char* NEW_NAMES[10] = { NULL };
char* OLD_NAMES[10] = { NULL };

/*
  Function Declarations for builtin shell commands:
 */
int changeDirectory(char **args);
int help(char **args);
int exitShell(char **args);
int setShellName(char **args);
int setTerminator(char **args);
int newName(char **args);
int listNewNames(char **args);
int saveNewNames(char **args);

/*
  List of builtin commands
 */
char *builtin_str[] =
{
    "cd",
    "help",
    "exit",
    "setshellname",
    "setterminator",
    "newname",
    "listnewnames",
    "savenewnames"
};

//Refrences to functions
int (*builtin_func[]) (char **) =
{
    &changeDirectory,
    &help,
    &exitShell,
    &setShellName,
    &setTerminator,
    &newName,
    &listNewNames,
    &saveNewNames
};

//Used to return how many commands there are
int lsh_num_builtins()
{
    return sizeof(builtin_str) / sizeof(char *);
}

/*
  Builtin function implementations.
*/

/**
   @brief   Bultin command: change directory.
   @param   args List of args.  args[0] is "cd".  args[1] is the directory.
   @return  Always returns 1, to continue executing.
 */
int changeDirectory(char **args)
{
    if (args[1] == NULL)
    {
        fprintf(stderr, "lsh: expected argument to \"cd\"\n");
    }
    else
    {
        if (chdir(args[1]) != 0)
        {
            perror("lsh");
        }
    }
    return 1;
}

/**
   @brief   Builtin command: print help.
   @param   args List of args.  Not examined.
   @return  Always returns 1, to continue executing.
 */
int help(char **args)
{
    int i;
    printf("The following are built in:\n");

    for (i = 0; i < lsh_num_builtins(); i++)
    {
        printf("  %s\n", builtin_str[i]);
    }
    
    printf("Other unix commands are also avalible \n");
    return 1;
}

/**
   @brief   Builtin command: exit.
   @param   args List of args.  Not examined.
   @return  Always returns 0, to terminate execution.
 */
int exitShell(char **args)
{
    return 0;
}

/**
   @brief   Builtin cmmand: change the shell name
   @param   args List of args.  Not examined.
   @return  Always returns 1, to continue executing.
 */
int setShellName(char **args)
{
    if (args[1] == NULL)    // If empty goes back to default
    {
        SHELL_NAME = "myshell";
    }
    else                    // Changes to new one
    {
        SHELL_NAME = args[1];
    }

    return 1;
}
/**
   @brief   Builtin command: change the terminator
   @param   args List of args.  Not examined.
   @return  Always returns 1, to continue executing.
 */
int setTerminator(char **args)
{
    if (args[1] == NULL)    // If empty goes back to default
    {
        TERMINATOR = ">";
    }
    else                    // Changes to new one
    {
        TERMINATOR = args[1];
    }

    return 1;
}

/**
   @brief   Builtin command: changes name of commands
   @param   args List of args.  Not examined.
   @return  Always returns 1, to continue executing.
 */
int newName(char **args)
{
    // Error handling if argument is empty
    
    if (args[1] == NULL)
    {
        fprintf(stderr, "lsh: expected argument to \"newname\"\n");
        return 1;
    }

    // If 3rd arguemnt is empty then delete alias
    if (args[2] == NULL)
    {

        for (int i = 0; i < NUM_OF_NEW_NAMES; i++)
        {
            // Will look for the matchting command and delete it while shifting the array afterwards
            if (NEW_NAMES[i] && strcmp(args[1], NEW_NAMES[i]) == 0)
            {
                for (int j = i; j + 1 < NUM_OF_NEW_NAMES; j++)
                {
                    NEW_NAMES[j] = NEW_NAMES[j + 1];
                    OLD_NAMES[j] = OLD_NAMES[j + 1];
                }
                
                NUM_OF_NEW_NAMES--;
                NEW_NAMES[NUM_OF_NEW_NAMES] = NULL;
                OLD_NAMES[NUM_OF_NEW_NAMES] = NULL;
                break;
            }
        }
    }
    
    else
    {
        // Add or update alias
        
        // Update alias
        for (int i = 0; i < NUM_OF_NEW_NAMES; i++)
        {
            if (NEW_NAMES[i] && strcmp(NEW_NAMES[i], args[1]) == 0)
            {
                OLD_NAMES[i] = args[2];
                return 1;
            }
        }
        
        // Insert new alias
        if (NUM_OF_NEW_NAMES >= 10)
        {
            fprintf(stderr, "lsh: alias limit reached\n");
            return 1;
        }
        
        NEW_NAMES[NUM_OF_NEW_NAMES] = args[1];
        OLD_NAMES[NUM_OF_NEW_NAMES] = args[2];
        NUM_OF_NEW_NAMES++;
    }
    
    return 1;
}

/**
   @brief   Builtin command: List new names/alias
   @param   args List of args.  Not examined.
   @return  Always returns 1, to continue executing.
 */
int listNewNames(char **args)
{
    int i;
    
    // Itterates through both arrays and prints to the screen
  
    for (i = 0; i < NUM_OF_NEW_NAMES; i++)
    {
        printf("  %s ", NEW_NAMES[i]);
        printf("  %s\n", OLD_NAMES[i]);
    }
    return 1;
}

/**
   @brief   Builtin command: saves the new names/alias to a file
   @param   args List of args.  Not examined.
   @return  Always returns 1, to continue executing.
 */
int saveNewNames(char **args)
{
    // Error handling if no file given
    if (args[1] == NULL)
    {
        fprintf(stderr, "lsh: Error no name entered\n");
        return 1;
    }
    
    // Creates or Opens the given file
    FILE *fp = fopen(args[1], "w");
    if (!fp)
    {
        perror("savenewnames");
        return 1;
    }

    // Prints names to the file
    for (int i = 0; i < NUM_OF_NEW_NAMES; i++)
    {
      if (NEW_NAMES[i] && OLD_NAMES[i])
      {
        fprintf(fp, "%s %s\n", NEW_NAMES[i], OLD_NAMES[i]);
      }
    }

  fclose(fp);
    
  return 1;
}

/**
  @brief    Launch a program and wait for it to terminate.
  @param args Null terminated list of arguments (including program).
  @return Always returns 1, to continue execution.
 */
int lsh_launch(char **args)
{
    pid_t pid;
    int status;

    pid = fork();
    
    if (pid == 0)
    {
    // Child process
        if (execvp(args[0], args) == -1)
        {
            perror("lsh");
        }
        exit(EXIT_FAILURE);
    }
    else if (pid < 0)
    {
        // Error forking
        perror("lsh");
    }
    else
    {
    // Parent process
        do
        {
            waitpid(pid, &status, WUNTRACED);
        }
        while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }

  return 1;
}

/**
   @brief Execute shell built-in or launch program.
   @param args Null terminated list of arguments.
   @return 1 if the shell should continue running, 0 if it should terminate
 */
int lsh_execute(char **args)
{
    int i;

    if (args[0] == NULL)
    {
    // An empty command was entered.
        return 1;
    }
    
    // checks for new name command
    for (i = 0; i < NUM_OF_NEW_NAMES; i++)
    {
        if (NEW_NAMES[i] && OLD_NAMES[i] && strcmp(args[0], NEW_NAMES[i]) == 0)
        {
            args[0] = OLD_NAMES[i];
            break;
        }
    }
    
    for (i = 0; i < lsh_num_builtins(); i++)
    {
        if (strcmp(args[0], builtin_str[i]) == 0)
        {
            return (*builtin_func[i])(args);
        }
    }

    return lsh_launch(args);
}

/**
   @brief Read a line of input from stdin.
   @return The line from stdin.
 */
char *lsh_read_line(void)
{
#ifdef LSH_USE_STD_GETLINE
    char *line = NULL;
    ssize_t bufsize = 0; // have getline allocate a buffer for us
    if (getline(&line, &bufsize, stdin) == -1)
    {
        if (feof(stdin))
        {
            exit(EXIT_SUCCESS);  // We recieved an EOF
        }
        else
        {
            perror("lsh: getline\n");
            exit(EXIT_FAILURE);
        }
    }
    return line;
    
#else
#define LSH_RL_BUFSIZE 1024
    int bufsize = LSH_RL_BUFSIZE;
    int position = 0;
    char *buffer = malloc(sizeof(char) * bufsize);
    int c;
    
    if (!buffer)
    {
        fprintf(stderr, "lsh: allocation error\n");
        exit(EXIT_FAILURE);
    }
    
    while (1)
    {
        // Read a character
        c = getchar();
        
        if (c == EOF)
        {
            exit(EXIT_SUCCESS);
        }
        else if (c == '\n')
        {
            buffer[position] = '\0';
            return buffer;
        }
        else
        {
            buffer[position] = c;
        }
        
        position++;
        
        // If we have exceeded the buffer, reallocate.
        if (position >= bufsize)
        {
            bufsize += LSH_RL_BUFSIZE;
            buffer = realloc(buffer, bufsize);
            if (!buffer)
            {
                fprintf(stderr, "lsh: allocation error\n");
                exit(EXIT_FAILURE);
            }
        }
    }
#endif
    
}

#define LSH_TOK_BUFSIZE 64
#define LSH_TOK_DELIM " \t\r\n\a"
/**
   @brief Split a line into tokens (very naively).
   @param line The line.
   @return Null-terminated array of tokens.
 */

char **lsh_split_line(char *line)
{
    int bufsize = LSH_TOK_BUFSIZE, position = 0;
    char **tokens = malloc(bufsize * sizeof(char*));
    char *token, **tokens_backup;
    
    if (!tokens)
    {
        fprintf(stderr, "lsh: allocation error\n");
        exit(EXIT_FAILURE);
    }
    
    token = strtok(line, LSH_TOK_DELIM);
    while (token != NULL)
    {
        tokens[position] = token;
        position++;
        
        if (position >= bufsize)
        {
            bufsize += LSH_TOK_BUFSIZE;
            tokens_backup = tokens;
            tokens = realloc(tokens, bufsize * sizeof(char*));
            
            if (!tokens)
            {
                free(tokens_backup);
                fprintf(stderr, "lsh: allocation error\n");
                exit(EXIT_FAILURE);
            }
        }
        
        token = strtok(NULL, LSH_TOK_DELIM);
    }
    
    tokens[position] = NULL;
    return tokens;
}

/**
   @brief Loop getting input and executing it.
 */
void lsh_loop(void)
{
    char *line;
    char **args;
    int status;

    do
    {
        printf("%s", SHELL_NAME);
        printf(" %s ", TERMINATOR);
        line = lsh_read_line();
        args = lsh_split_line(line);
        status = lsh_execute(args);

   
    }
    while (status);
}

/**
   @brief Main entry point.
   @param argc Argument count.
   @param argv Argument vector.
   @return status code
 */
int main(int argc, char **argv)
{
  // Load config files, if any.
  
  // Run command loop.
    lsh_loop();

  // Perform any shutdown/cleanup.

  return EXIT_SUCCESS;
}





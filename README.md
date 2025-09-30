# ToyShell made in C 

## Discription 

This was a project for Operating Systems class that used Libstephen SHell as the base code with other feature having to be added. The program has basic UNIX commands as well as error handling. Thr program is on a single C file. It compiles and runs with no dependecies.  

## Rubric 

a. (2 marks) STOP: Terminates execution of the current myshell session.


b. (2 marks) SETSHELLNAME <shell_name>: Sets the shell name in the myshell command
prompt to <shell_name>. For example, a sample command prompt is shown below:
myshell> In this command prompt, there are two parts. The first part, myshell, is the shell name. The
second part, >, is the terminator. If no shell name is defined, myshell should be the default
shell name.

c. (2 marks) SETTERMINATOR <terminator>: Sets the terminator in the myshell command
prompt to <terminator>. If no terminator is defined, myshell should use > as the default
terminator.

d. (8 marks) NEWNAME <new_name> | <new_name> <old_name>: Manages the alias list.
The first option deletes a previously defined alias. The second option defines an alias for an
existing command. For example, the command NEWNAME mycopy cp defines mycopy as
the alias for the cp command. The command NEWNAME mycopy deletes the alias
mycopy. If an alias for a command already exists, then the new alias replaces the old alias.
The maximum number of aliases in the alias list should be set to 10 as the default.

e. (5 marks) LISTNEWNAMES: Outputs all the aliases that have been defined. Each pair of
names should be shown on one line. For example, the possible aliases for a few commands
are shown below:
mycd cd
mycopy cp

f. (5 marks) SAVENEWNAMES <file_name>: Stores all currently defined aliases in the file
<file_name>.

g. (5 marks) READNEWNAMES <file_name>: Reads all aliases in the file <file_name>.

h. (1 marks) <UNIX_command>: Executes the UNIX command <UNIX_command>,
corresponding to any valid UNIX command. If the first token on a command line is not a
built-in command, assume that it is a UNIX command.

i. (1 marks) HELP: shows all built-n commands supported by myshell.


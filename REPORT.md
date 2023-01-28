# ECS 150 Shell Terminal Report
# By Nayeel Imtiaz and Ky Ngo








## Process object: What and why?

Before diving into the details, our project implements an important 

structure we called “Process”, which contains fields like string 

“program name” and linked list “arguments”. As an example: given a 

user entered command, 


**Ex 1: [echo Hello World]**


we break the string down at space characters (‘ ‘) and create a 

process object that contains “program name” ‘echo’ and “arguments” 

‘Hello’ and ‘World’. As another example, given a complex string

with pipes (‘|’),


**Ex 2:[echo Hello World | grep Hello | wc -l]**


we first break the string where pipes are found, leaving us with 

three new individual strings. Each of these strings is then broken 

down into individual “Process” objects using the same process as 

the first string, leaving us with three process that contain 

necessary information for all three commands. It is here that we 

hope the design choice to use process objects becomes clear. By 

using process objects, it becomes easy to chain commands together 

or run them concurrently because we have access to objects that 

encapsulate all of the desired commands at runtime. For simple 

strings like the first example, only one process object is needed 

and it is trivial to execute the desired command using execvp(). 

For more complex strings like the second example, we are able to 

perform piping concurrently versus if we had read the input string 

from beginning to end. As a final example for a third type of

string, given a string with redirection operators ‘>’ or ‘>>’:


**Ex 3: [echo Hello World > output.txt]**


the program considers everything to the left and right of the 

redirection operator to be one process. The "Process" object 

contains one additional field, a string "file name", that holds the 

strings to the right of the redirection operator.


## Our project: A description 

With this “Process” object in mind, upon receiving an input string

into the “sshell”, our program can be separated into two main 

stages: reacting to input and running the executed command. 


#### Stage one: input validation and parsing 

In the first stage, assuming valid input, a user entered string is 

broken down into as many “Process” objects as necessary, which are 

stored for running commands in the future. Given an invalid input 

string, the program requests new input and restarts from stage 1. 

The two relevant functions at this stage are ```split_pipes()```,

which splits user input based on the number of pipes if any, and 

```split_redirection```, which takes the split strings, looks for 

redirection operators using the steps in Ex 3, and creates 

"Process" objects. 


#### Stage 2: Running desired commands

In the second stage, we can now execute the desired commands using

the “Process” objects. Stated earlier, one process object indicates 

a single command and a single fork() operation. The child process

executes the command contained in “Process” using execvp() and the 

parent waits for the child process to end, after which the program

resets to stage one and requests new user input. The function 

```shell_system()``` handles one process. The presence of two or

more process objects indicates that pipes were found when breaking

down the string(Ex 2) and is handled by ```shell_system_pipe()```.

In this case, we need to run the processes concurrently as a

regular shell would. This is done by forking as many times as there 

are “Process” objects, redirecting output between each of the child 

processes, additional running one desired command per process 

object. The exceptions to this process are the built-in commands

(```cd```, ```pwd```, and```exit```) , which do not require the 

creation of “Process” objects and are simply executed upon 

detection in stage 1. 


## Testing:

Our first priority was to pass the tester.sh file provided to us,

so we fixed errors in accordance to test cases that did not pass. 

We then manually tested the functionalities of each of the phases 

outlined in the project1.html page. While we had many errors in the

development process, most of our errors were either SegFaults or 

memory leaks because we used allocated memory and used pointers 

extensively throughout the project.


## References:

Lecture slides 

[General C programming](https://stackoverflow.com/)

[Linux man pages](https://linux.die.net/man/) 

[Professor’s Markdown cheetsheet](https://github.com/adam-p/
markdown-here/wiki/Markdown-Cheatsheet) 
 
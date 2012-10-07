Questcode
=========

This was a class project carried out by Jorge Diz Pico and Marcelino Alberdi Pereira at the beginning of 2009 for the Compilers course of the fourth year of the Computer Science Engineering degree from the University of A Coruña

### Introduction


*A language based on roleplaying combats*

The goal of this program is to implement an integer-handling language whose code simulates a classic fantasy or roleplaying combat. For instance:

> A level 5 paladin enters the battlefield.

> As long as the paladin is alive.

>  Follow tactic.

>  Scry on paladin.

>  1 goblin damages the paladin.

>  End tactic.

> Scry on paladin.

(file: `examples/loop`)

Which decrements a variable from 5 to 0, printing it on the screen.

The inner workflow of the system is as usual. First, the code on the input file is processed by the lexer, identifying and classifying the language tokens.

Then, they are given to the parser to check for grammar validation. At the same time, it generates a statement tree, storing the actions (commands) to carry out and over whom. It is in this stage where syntax errors and the most common malformations are detected (use of a keyword as variable name, structural incoherence, etc).

Once the code has all been read and the correct grammar tree has been generated, the action execution process can begin. As the nodes are traversed, the variables are created, operated upon, read or print, as requested.

Take notice that this `compiler`s behavior is more akin to an interpreter, since it is in this execution stage where errors such as double declaring a variable, read a nonexistent one or division by zero are detected. For instance, this code:

> Scry on orc.

(file: `examples/nodecl`)

generates the following error output:

> $ ./qc < examples/nodecl

> Error: I can't find orc (are you sure it's in the battlefield?).

> Story finished.

Warning that the variable (the orc) has not been declared previous to the attempted access (it is not `on the battlefield`).

Finally, after execution, a memory cleaning takes place by the garbage collector.


### Source files

This program comprises the following source files:

- `qc.l`

Lexical scanner in LEX. Processes the file into tokens, discards comments and sends the chunks to YACC.

- `qc.y`

Implements the language grammar in BNF. Traverses the tokens checking for structural coherence, identifies the statements and creates the objects accordingly. Detects syntax errors and, in their absence, asks for the commands' execution.

- `mydatatypes.h`

Defines all datatypes used in the compiler such as statements, symbols, variables, …

- `mygcollector.h`

Garbage collector, keeps a list of the addresses to free.

- `mysentences.h`

Functions dealing with creating and handling statements and blocks of them.

- `mycommands.h`

Functions dealing with creating and handling commands (actions to be performed by each statement).

- `mysymboltable.h`

Functions dealing with creating and handling variables and the context in which they are stored.

- `myinout.h`

Functions dealing with outside communication for the compiler (both parameter reading and screen printing).

- `myexecution.h`

Program flow through execution of the commands from the statements.


### Compilation, use and examples

For compiling, make use of the Makefile with the command `make` (note the possibility of performing `make clean` too).

To run the program, write the following line on a terminal:

> ./qc < examples/TARGET_FILE

It must be pointed out that, despite it's nonchalant look, the language is Turing-complete.
As a sample of its possibilities, the file examples/fib contains a poetic Fibonacci implementation:

> \# Returns the nth element of the fibonacci sequence.

> A happy level 1 FemaleRabbit enters the battlefield.

> A hopeful MaleRabbit enters the battlefield.

> Some LittleRabbits enter the battlefield.

> The Sun enters the battlefield.

> Command Sun.

>

> As long as The Sun is alive.

>  Follow tactic.

>    0 goblins cheer the LittleRabbits.

>    MaleRabbit heals the LittleRabbits.

>    FemaleRabbit heals the LittleRabbits.

>    The FemaleRabbit confronts the MaleRabbit.

>    The LittleRabbits confront the FemaleRabbit.

>    1 goblin damages The Sun.

>  End tactic.

> The MaleRabbit confronts the LittleRabbits. # We did two extra iterations
> Scry LittleRabbits.
> \# End.

This is a small summary of the provided examples you can find. Those marked with an * need a command-line provided parameter.

- `*context`
-- shows how two variables may share a name and how the one declared in the innermost block is always used

- `divbyzero`
-- produces an error when trying to divide by zero

- `*fib`
-- already described, calculates the n-th element of the fibonacci series in a not really efficient way

- `fireball`
-- raises a syntax error when finding a string non accepted by the grammar

- `loop`
-- a simple loop decreasing a variable

- `nodecl`
-- raises the error produced when trying to act upon a non declared variable

- `*surrender`
-- reads a variable from command line, and then prints it if greater than zero, or terminates otherwise

Thanks for reading and we hope you enjoy it!

<font color="#4b85eb"><h1>C Project: Algorithmic language interpreter </h1>


This project's aim is to understand the algorithmic language directly in order to be able to implement an interpreter of this language.

Our project must be able to be compiled on Windows (7, 8 & 10 at least) and Linux (Ubuntu 16.04 LTS and AntiX 16) with gcc compiler.

<font color="#6b7979">* To do so, the user who wants to compile the program has to change a #define in constants.h in the headers folder.
	* If working with a Windows OS, the user must write `#define WINDOWS` and delete `#define LINUX` (if it was written)</li>
	* If working with a UNIX based OS, the user must write `#define LINUX` and delete `#define WINDOWS` (if it was written)

<font color="#f50606">EDIT: You no longer need to change the file constants.h to make the program works on both Unix and Windows OS, the program detects the current OS by itself since release 1.1.0

The executable ExeTest which is provided was obtained by compiling all the sources (.c) files in the sources folder with gcc compiler:
* The exact command used was (located on the project's main folder):
<br />`gcc sources/*.c -o ExeTest -Wall`

* The exact command used to debug out program (with *valgrind*) was:
<br />`valgrind --leak-check=full --track-origins=yes --show-leak-kinds=all ./ExeTest`

---------------------------------------
_If you see any mistake on our code, or a more efficient way to do it, please feel free to leave a message in the "Issues" or "Projects" sections._


Thanks for reading, enjoy exploring the project, or using our program

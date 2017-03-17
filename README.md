<h1>C Project: Algorithmic language interpreter </h1>


This project's aim is to understand the algorithmic language directly in order to be able to implement an interpreter of this language.

Our project must be able to be compiled on Windows (7, 8 & 10) and Linux (Ubuntu 16.04 LTS and AntiX 16) with gcc compiler.
<ul>
<li>To do so, the user who wants to compile the program has to change a #define in constants.h in the headers folder.
	<ul>
	<li>If working with a Windows OS, the user must write <code>#define WINDOWS</code> and delete <code>#define LINUX</code> (if it was written)</li>
	<li>If working with a UNIX based OS, the user must write <code>#define LINUX</code> and delete <code>#define WINDOWS</code> (if it was written)</li>
	</ul>
</li>
</ul>

The executable ExeTest which is provided was obtained by compiling all the sources (.c) files in the sources folder with gcc compiler:
<ul>
<li>The exact command used was (located on the project's main folder): <code>gcc sources/*.c -o ExeTest -Wall</code></li>
</ul>

---------------------------------------
If you see any mistake on our code, or a more efficient way to do it, please feel free to leave a message in the "Issues" or "Projects" sections.


Thanks

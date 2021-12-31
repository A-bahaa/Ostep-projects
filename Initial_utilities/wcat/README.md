# Wcat
Wcat is similar to cat command, a C++ program that opens a file and reads its contents.
We can make a source file wcat.c and write the c program, then we first must compile it.
```
g++ wcat.cpp -o wcat -Wall -Werror
```
g++ command is used to compile mainly C++ language, -o flag is used for output file option, -Wall flag check for errors and warnings like unused variables warnings, -Werror flag shows if any errors found in the code.
After this we must test the file “wcat” to run the C++ code.

![ىىى](https://user-images.githubusercontent.com/66278563/147831230-9db0fbe9-a6a7-4c7e-94f4-94823d375eb9.png)
# Example

if we have a file named **hello** and have a text **Hello World**
to run the program and open the file (hello), we write this command in terminal:
```
./wcat hello
```
the output must be:
```
Hello World
```

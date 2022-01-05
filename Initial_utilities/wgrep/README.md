# Wgrep

Wgrep is a tool looks through a file, line by line, trying to find a user-specified search term in the line. 
If a line has the word within it, the line is printed out, otherwise it is not.

## Example
 consider you have a file named **test** that has 
```
this line has foo in it
this one doesn't
so does this foolish line; do you see where?
```
written in it, You will write in the terminal as follows
```
./wgrep foo test.txt
```
the output will be as the following: 
```
this line has foo in it
so does this foolish line; do you see where?
```
## Notes:
* If you want to search through multiple file, You will write in the terminal as follows:
```
./wgrep foo test.txt blep.txt blop.txt
```
* If there is a search term, but no file is specified, wgrep should read from standard input.
And the command would be written as follows:
```
./wgrep foo
```
## How to run and test your program ?

Open the terminal and compile your program by  `g++ wgrep.cpp -o wgrep -Wall -Werror` command  .

If you want to run `wgrep` on a file in your device use this command  `./wgrep SearchTerm file1.txt ... file(n).txt` .

To run all test cases use `./test-wgrep.sh` command. Look at this [README] (https://github.com/remzi-arpacidusseau/ostep-projects/blob/master/initial-utilities/wgrep/README.md) .

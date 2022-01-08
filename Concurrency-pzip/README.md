# Parallel Zip
The goal of this project is to use multiple threads efficiently and this will return to the performance of the program in terms of time ( high performance compared to the sequential zip).

## Example
 consider you have a file with the following input: 
```
aaaaabbbbbcccccaaaaa
```
the output will be as the following: 
```
5a5b5c5a
```
## How to run and test your program ?

Open the terminal and compile your program by  `g++ pzip.cpp -o wunzip -Wall -Werror -pthread -O` command  .

If you want to run `pzip` on a file in your device use this command  `./pzip file_name.txt` .

To run the test samples type `./test-pzip.sh` . See [README](https://github.com/remzi-arpacidusseau/ostep-projects/blob/master/tester/README.md) for the possible test options .

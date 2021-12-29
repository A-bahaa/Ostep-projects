# Wzip

As the name suggests , `Wzip` is a simple file compression tool based on [run-length encoding (RLE)](https://en.wikipedia.org/wiki/Run-length_encoding#Example) .

## Example
if the file you pass for example is 
```
aabaac

ttt  QQQ
```
Then the compression `wzip` produce is 
```
2a1b2a1c2
3t2 3Q
```
## How to run it

In the terminal type `g++ wzip.c -o wzip -Wall -Werror` to compile , then `./wzip file1.txt ... file(n).txt` to run `wzip` on `n` files of your choice .

To run the test samples type `./test-wzip.sh` . See [README](https://github.com/remzi-arpacidusseau/ostep-projects/blob/master/tester/README.md) for the possible test options . 

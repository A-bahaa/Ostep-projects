#include <bits/stdc++.h>
using namespace std;
string x , y ;
char chr ;
bool ok ;

/*
	This function tell me if a string x is a substring of y or not ... if (true) return 1 else return 0 .
        Firstly if the string x has a size that is bigger than the size of y it is impossible to return 1 .
	In case of x.size()<=y.size() i will consider it a simple problem and apply my algorithm to solve this problem .
        There are many algorithms to solve this problem like Hashing , .... etc but for simplicity i will use bruteforce technique .
*/
bool is_matching(string y, string x){
    if(x.size() > y.size())
        return 0;

    for (int i = 0; i <(int)y.size(); i++){
        int j = 0;
        if(y[i] == x[j]){
            int pos = i;
            while (y[i] == x[j] && j <(int)x.size()){
                j++;
                i++;
            }
            if (j == (int) x.size())
                return 1;
            else
                i = pos;
        }
    }
    return 0;
}
int main(int argc, char **argv)
{
    if (argc == 1)  /// In case of no input .
    {
        cout << "wgrep: searchterm [file ...]\n";
        return 1;
    }
    FILE *file = freopen(argv[2], "r", stdin);
    if (argc == 3)
    {
        if (file == NULL)  /// In case of the file can not be open .
        {
            cout << "wgrep: cannot open file\n";
            return 1;
        }
    }
     x = argv[1];           /// The string that i want to found it in a line .
     chr = 'a';    	     ///  Character initialization to avoid carrying a garbage .
    while (chr != EOF)     /// Do work until the EOF .
    {
        if (chr == '\n'){  ///In case of chr = '\n' get a character from the next line and continue .
            chr = getchar();
            continue;
        }
        getline(cin, y);  /// Take the line that you will search in .
        y = chr + y;      /// Concatenate your chr with the line you get .
        if (is_matching(y , x))   /// Check if the string is found at the line .
        {
            y.erase(y.begin());   /// Remove the first element from the line
            if (ok)               /// First time ? do not print the chr and print line only : print the chr and the line
                cout << chr;
            cout<<y<<'\n';
        }
        chr = getchar();  /// Get the next chr
        ok=1;             /// After first chr ok will be = 1 all the time
    }
    return 0;
}

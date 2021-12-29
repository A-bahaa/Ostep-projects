#include <bits/stdc++.h>
using namespace std;
#define ll long long
char letter ;
int char_rep ;
vector<pair<int,char> > print ;   	   /// It is a vector of pairs >> each pair contains the number of char_rep and the char itself . 
int main(int argc, char** argv)      /*The argc contains the number of arguments >> argv is an array of strings that contains the arguments
						itself . */
{
    if(argc==1)  			   /// In case of no files .
    {
        cout<<"wunzip: file1 [file2 ...]\n";
        return 1;                        /// Means that there is some error while executing the program such as "No files as an input" .
    }
    for(int i=1; i<argc; i++)
    {
        FILE *fp;   			  /// Pointer to file .
        fp=freopen(argv[i],"r",stdin);  /// Open the file to read from it .
        print.clear(); 		 /// Afer ending the file clear the vector to do work again to the next file .
        while(fread(&char_rep, 1, sizeof(char_rep), fp )) /// Read a number from the file in binary mode .   
        {
            fread(&letter, 1, sizeof(letter), fp);	   ///  Read a character from the file in binary mode .
            print.push_back({char_rep,letter});         ///   Push {the number of char_rep , and the char itself} to the vector .
        }
	/*
	Go to each element in the vector .
	We found that each element has a pair = {the number of char_rep , and the char itself} .
	We will print the char by the number of the char_rep .	
	*/
        for(int i=0 ; i<(int)print.size() ; i++)       
        {
            for(int j=0 ; j<print[i].first ; j++)
                cout<<print[i].second;    
        }
        fclose(fp);  /// close the file .
    }
    return 0;
}


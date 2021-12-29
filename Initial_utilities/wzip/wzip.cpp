#include <iostream>
#include <fstream>
#include <bits/stdc++.h>
using namespace std;


vector<pair<int,char>>ans;
// string s holds the content of the file one line at a time and get zipped by the zip function
string s;

void zip(){

  int count = 1;
  for(int i = 1; i<(int)s.size(); i++){

    //the same char repeats
    if(s[i] == s[i-1]) count++;
    else{
      //zip the last stream
      ans.push_back({count,s[i-1]});
      //reinitialize the count
      count = 1;
    }
  }
  //zip the last stream
  ans.push_back({count,s[s.size()-1]});
    
}

int main(int argc , char** argv){

  //if no file is passed , raise the following exception as is advised in the project README file
  if(argc==1){
    cout<<"wzip: file1 [file2 ...]\n";
    return 1;
  }
  //current_char , previous_char
  char cc , pc = '1';

  for(int i=1;i<argc;i++){
    FILE* file;

    file = freopen(argv[i],"r",stdin);
    
    cc=getchar();
    
    while(cc!=EOF){
      // if we keep getting a newline .
      if(pc=='\n' && cc=='\n'){
        //since we zip the file one line a time , we sure that the last pair in the ans vector is a new line pair 
        //so just increment the count for the newline char
        ans[ans.size()-1].first++;
        //process the next char
        cc=getchar();
        continue;
      }
          
      //if we reach the newline char    
      if(cc=='\n'){
        //invoke the zip function
        zip();
        //push_back the newline pair
        ans.push_back({1,'\n'});
        //clear the s   
        s.clear();
        pc=cc;
        cc=getchar();
        continue;
      }
      //if we didn't reach the endline char keep appending in the string s
      s += cc;
      pc = cc;
      cc = getchar();
    }
    fclose(file);
  }
  if(!s.empty())zip();
  for(auto p:ans){
    fwrite(&p.first, 1 , sizeof(p.first) , stdout);
    fwrite(&p.second, 1 , sizeof(p.second) , stdout);
  }

  return 0;
}




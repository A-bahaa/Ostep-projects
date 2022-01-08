#include <iostream>
#include <fstream>
#include <bits/stdc++.h>
#include <pthread.h>
#include <signal.h>
using namespace std;
pthread_mutex_t lockk=PTHREAD_MUTEX_INITIALIZER;
int pthread_mutex_lock(pthread_mutex_t*mutex);
int pthread_mutex_unlock(pthread_mutex_t*mutex);
pthread_t threads[10];
struct cur_struct{
    FILE *file;
    int index;
}x[50];
vector<pair<int,char> >ans[50];
char c;
string tmp;

void *zip(void *file){

  cur_struct cur = *(cur_struct*)file;     /// casting a void file to cur_struct
  int idx = cur.index;                     /// contain the index of this file
  FILE *cur_file = cur.file;              ///  contain file itself
  pthread_mutex_lock(&lockk);           /// LOCK before the critical section
  
  /*
  this algorithm is to compress an input file 
  */
    tmp.clear();
    while((c = fgetc(cur_file)) != EOF)
        tmp += c;
  int count = 1;
  for(int i = 1; i<(int)tmp.size(); i++){

    if(tmp[i] == tmp[i-1]) count++;
    else{
      ans[idx].push_back({count,tmp[i-1]});
      count = 1;
    }
  }
  ans[idx].push_back({count,tmp[tmp.size()-1]});
  pthread_mutex_unlock(&lockk);  /// UNLOCK after finishing the critical section
  fclose(cur_file);              /// close the file
  return NULL ;                  /// return nothing
}

int main(int argc , char** argv){

  if(argc==1){                       /// in case of no files as an input.
    cout << "pzip: file1 [file2 ...]\n";
    return 1;
  }
  for(int i = 1;i < argc;i++){
        
        /// each struct will contain a pointer to a file and the index of this file
        x[i].file = fopen(argv[i],"r");
        x[i].index = i;
    }
    for(int i=1;i<argc;i++){
        pthread_create(&threads[i],NULL,zip,&x[i]); /// for each file create a thread to compress it.
    }

    for(int i=1;i<argc;i++){
        pthread_join(threads[i],NULL);   /// parent thread will wait for all child threads to do  work firstly.
    }
    
    /*
        The next section is to solve the following problem :
        consider i have two files : file-1 contain : aaaaa  && file-2 contain aaaaa .
        we should print 10a not 5a5a .
    */
    for(int i=2; i<argc; i++)
    {
        int sz1=ans[i-1].size()-1;
        if(ans[i-1][sz1].second==ans[i][0].second)
        {
            ans[i][0].first+=ans[i-1][sz1].first;
            ans[i-1].pop_back();
        }
    }
    
    /*
    the next section is to print output in file in binary mood .
    */
    for(int i = 1;i < argc ;i++){
        for(auto it:ans[i])
        {
        	 fwrite(&it.first,1,sizeof(it.first),stdout);
        	 fwrite(&it.second,1,sizeof(it.second),stdout);
        }
    }
  return 0;
}








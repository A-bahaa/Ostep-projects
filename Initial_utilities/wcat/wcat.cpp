#include <iostream>
#include <fstream>
using namespace std;

int main(int argc, char **argv)
{

/* argc=1 means there's no file name */

	if(argc==1){
    	return 0;
    }

    for (int i = 1; i < argc; i++){
/* ifstream is stream class to read from files */

/* argv[i] is listing every element in command line */

    	ifstream file(argv[i]);

/* is_open() checks file is open or not */

		if (file.is_open()){
    		string input;
			while (getline(file , input)){
				cout<<input<<'\n';
			}
		}

	 	else{
	 		cout<<"wcat: cannot open file"<<endl;
	 		return 1;
		}
    }
    return 0;
}

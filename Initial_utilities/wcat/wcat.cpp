#include <iostream>
#include <fstream>
using namespace std;

int main(int argc, char **argv)
{

/* argc=1 means there's no file name */

	if(argc==1)
        {

    	return 0;
        }

    for (int j = 1; j < argc; j++)
    {

/* ifstream is stream class to read from files */

/* argv[j] is listing every element in command line */

         ifstream file(argv[j]);

/* is_open() checks file is open or not */


	 if (file.is_open())
         {
/* rdbuf() gets stream buffer */

		cout << file.rdbuf();
         }

	 else{
	 	cout<<"wcat: cannot open file"<<endl;
	 	return 1;
	 }
    }
    return 0;

}

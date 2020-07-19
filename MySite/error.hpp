#include <iostream>
#include <string>

using namespace std;

#ifndef __ERROR_HPP__
#define __ERROR_HPP__

namespace ERROr{
	void err(string error_message){
		cout<<"["<<__FILE__<<"] ["<<__LINE__<<"] "<<error_message<<endl;
	}
}

#endif
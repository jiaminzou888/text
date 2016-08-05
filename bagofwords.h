/*文本分析算法*/
#ifndef _BAGOFWORDS_H
#define _BAGOFWORDS_H
#include <math.h>
#include "../commom/func.h"
#include <algorithm>
namespace text{
	class Bagwords{
	public:
		std::map<string,int>dict;
		std::map<string,int>stopwd;
		int vcabsize;
	public:
		Bagwords(){
			vcabsize = 0;
		}
		~Bagwords(){}
		//init dict
		bool Init(const char* filepath,const char* stoppath);

		//filter stop words
		string Filter(string str);

		//caculate strvector
		bool Count(float* vec,const char* str);

		//write file
		bool Writefile(const char* filepath, const char* outpath);
	};

}
#endif
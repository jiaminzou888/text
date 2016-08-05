/*文本分析算法*/
#ifndef _TEXTVECTOR_H
#define _TEXTVECTOR_H
#include <math.h>
#include "../commom/func.h"
#include "time.h"
#include <algorithm>
namespace text{
	//文本向量算法类
	class Textvector{
	public:
		int m_size;
		int m_vocab;
		float** dict;
		std::map<std::string, int>strint;
		std::map<int, std::string>intstr;
	public:
		Textvector();
		~Textvector();
		bool LoadVector(int x,const char* filepath);		

		float* GetStrVector(std::string& str);

		float Distance(std::string& str, std::string&  word);
		
		float Distance(float* v, std::string  word);		
		
		bool Train(float* pravec,std::string& str);		

		bool GetCenter(float* v,  std::vector<std::string>& vec);

		bool Writefile(const char* filepath, const char* outpath);
	};
}
#endif
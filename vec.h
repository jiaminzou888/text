#ifndef _VEC_H
#define _VEC_H
#include <math.h>
#include "../commom/func.h"
#include <algorithm>
namespace text{
	//文本向量算法类
	class Vector{
	public:
		int m_size;
		int m_vocab;
		float** dict;
		std::map<std::string, int>strint;
		std::map<int, std::string>intstr;
	public:
		Vector();
		~Vector();
		//加载词典
		bool LoadVector(const char* sp, const char* filepath);	
		/*
		//获取词语向量
		float* GetStrVector(std::string& str);
		//计算余弦距离
		float Distance(std::string& str, std::string&  word);
		float Distance(float* v, std::string  word);		
		//训练STR向量
		bool Train(float* pravec,std::string& str,int flag);		
		//获取中心词
		bool GetCenterWords(int n,const char* path);
		//计算余弦距离TOPN
		bool TopCosin(std::string& str, std::vector<wdlen>& vec, int n);
		bool TopCosin(float* v,  std::vector<wdlen>& vec , int n);			
		//获取中心向量
		bool GetCenter(float* v,  std::vector<std::string>& vec);
		*/
		//K-Means聚类
		bool Kmeans(int k, int  iter,const char* outpath);
	};
}
#endif
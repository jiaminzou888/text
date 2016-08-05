#include "bagofwords.h"
#include "../commom/func.h"
namespace text{
	bool Bagwords::Init(const char* filepath,const char* stoppath){
		char buffer[MAX_LENTH];	
		FILE* fi = fopen(filepath,"r");
		if(fi == NULL){
			commom::LOG_INFO("open file error");
		}
		std::string str;
		std::vector<string> v;
		while ( commom::ReadLine(buffer,MAX_LENTH,fi)!=NULL){
			str = commom::GetLine(buffer);
			commom::Split(" ", str, v);
			for(int i =0; i< v.size(); i++){
				if(v.at(i) != " "){
					dict[v.at(i)]++;
				}
			}
		}
		fclose(fi);
		std::vector<std::pair<string,int> > tmp;
		for(std::map<string,int>::iterator it = dict.begin(); it != dict.end(); it++){
			tmp.push_back(*it);
		}
		sort(tmp.begin(), tmp.end(), commom::SortBySecondGreater);
		dict.clear();
		for(int i = 0; i<tmp.size(); i++){
			if(tmp.at(i).second > 10){
				dict[tmp.at(i).first] = i;
			}
		}	
		vcabsize = dict.size();
		fi = fopen(stoppath,"r");
		if(fi == NULL){
			commom::LOG_INFO("open file error");
		}
		while ( commom::ReadLine(buffer,MAX_LENTH,fi)!=NULL){
			str = commom::GetLine(buffer);
			commom::Split(" ", str, v);
			for(int i =0; i< v.size(); i++){
				if(v.at(i) != " "){
					stopwd[v.at(i)]++;
				}
			}
		}
		fclose(fi);
		return true;
	}
	string Bagwords::Filter(string str){
		string s = "";
		std::vector<string> v;
		commom::Split(" ", str, v);
		for(int i =0; i< v.size(); i++){
			if(stopwd.find(v.at(i)) == stopwd.end()){
				if(s == ""){
					s = v.at(i);
				}else{
					s += (" " + v.at(i));
				}
			}
		}
	}

	bool Bagwords::Count(float* vec,const char* str){
		std::vector<string> v;
		commom::Split(" ", str, v);
		for(int i =0; i< v.size(); i++){
			if(dict.find(v.at(i)) == dict.end()){
				continue;
			}else{
				vec[dict[v.at(i)]] += 1; 	
			}			
		}
		//¹éÒ»»¯
		float sz = 0;
		for(int j =0; j< vcabsize; j++){
			sz += vec[j]*vec[j];
		}
		for(int j =0; j< vcabsize; j++){
			vec[j] /= sz;
		}
	}
	bool Bagwords::Writefile(const char* filepath, const char* outpath){			
		FILE* fi = fopen(filepath,"r");
		FILE* fo = fopen(outpath, "ab+");
		if((fi == NULL)|(fo == NULL)){
			commom::LOG_INFO("open file error");
		}
		char buffer[MAX_LENTH];
		std::string str;
		float* vec = new float[vcabsize];
		while ( commom::ReadLine(buffer,MAX_LENTH,fi)!=NULL){
			str = commom::GetLine(buffer);
			Count(vec, str.c_str());
			str = commom::Unit(str);
			str += "\3";
			for(int i = 0; i< vcabsize-1; i++){
				str += (commom::ConvertToStr(vec[i]) + " ");
			}
			str += commom::ConvertToStr(vec[vcabsize-1]);
			str += "\n";
			commom::WiteLine(str.c_str(), fo);
		}
		fclose(fi);
		fclose(fo);
	}
}
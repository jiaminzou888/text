#include "textvector.h"
#include "../commom/func.h"
namespace text{
	Textvector::Textvector(){
		m_size = 0;
		m_vocab = 0;
		dict = NULL;
	}
	Textvector::~Textvector(){
		for(int v = 0; v<m_vocab; ++v){
			delete[]dict[v];
		}
		delete[] dict;
	}

	bool Textvector::LoadVector(int x, const char* filepath){
		m_size = x;
		FILE *fi = fopen(filepath,"r");
		if (fi == NULL){
			commom::LOG_INFO(std::string(filepath)+ ":open file error ");
			return false;
		}
		char buffer[MAX_LENTH];

		int linenum = 0, line =0;
		while (commom::ReadLine(buffer,MAX_LENTH,fi)!=NULL){	
			linenum++;
		}
		fclose(fi);
		m_vocab = linenum;
		fi = fopen(filepath,"r");
		if (fi == NULL){
			commom::LOG_INFO(std::string(filepath)+ ":open file error ");
			return false;
		}
		std::vector<std::string> words;
		dict = new float*[linenum];
		for (int i=0; i<linenum; i++){	
			dict[i] = new float[m_size];	
			memset(dict[i],0,sizeof(float)*m_size);
		}
		while ( commom::ReadLine(buffer,MAX_LENTH,fi)!=NULL)	{	
			line++;
			std::string str =commom::GetLine(buffer);
			commom::Split(" ", str, words);
			if(words.size() <= m_size)continue;
			intstr[line-1] = words.at(0);
			strint[words.at(0)] = line;
			for(int i = 1; i<= m_size; i++){
				dict[line-1][i-1] = atof(words.at(i).c_str());
			}
		}
		commom::LOG_INFO(commom::ConvertToStr(intstr.size()));
		commom::LOG_INFO(commom::ConvertToStr(strint.size()));
		fclose(fi);
		return true;
	}


	float* Textvector::GetStrVector(std::string& str){
		if(strint.find(str) == strint.end()) return NULL;
		return dict[strint[str] - 1];
	}

	float Textvector::Distance(std::string& str, std::string&  word){
		float distence = 0;
		if(strint.find(str) == strint.end()) {
			return 0;
		}
		if(strint.find(word) == strint.end()){
			return 0;
		}
		float d1 = 0,d2 = 0;
		float c1 = 0, c2 =0;
		for (int j = 0; j< m_size; j++){
			c1 = dict[strint[str] - 1][j];
			c2 = dict[strint[word] - 1][j];
			distence += c1 * c2;
			d1 += c1*c1;
			d2 += c2*c2;
		}
		d1 = sqrt(d1);
		d2 = sqrt(d2);
		return distence/(d1*d2);
	}

	float Textvector::Distance(float* v, std::string  word){
		float distence = 0;
		if(strint.find(word) == strint.end()) return 0;
		float d1 = 0, d2 =0;
		float c1 = 0, c2 =0;
		for (int j = 0; j< m_size; j++){
			c1 = v[j];
			c2 = dict[strint[word] - 1][j];
			distence += c1 * c2;
			d1 += c1*c1;
			d2 += c2*c2;
		}
		d1 = sqrt(d1);
		d2 = sqrt(d2);
		return distence/(d1*d2);
	}

	//训练文本向量
	bool Textvector::Train(float* pravec,std::string& str){
		float* h = new float[m_size];
		memset(h, 0, sizeof(float)*m_size);
		if(str == "")return false;
		std::vector<std::string> words;
		commom::Split(" ", str, words);
		if(words.size() == 0)	return false;
		if(GetCenter(pravec, words) == false) return false;
		for(int jj =0; jj< words.size(); jj++){
			if(strint.find(words.at(jj)) != strint.end()){
				float g = 0,  alpha = 0.03;
				float d = 0;
				float d1,d2 =0;
				float c1, c2 =0;
				for (int j = 0; j<m_size; j++){
					c1 = pravec[j];
					c2 = dict[strint[words.at(jj)] - 1][j];
					d += c1*c2;
					d1 += c1*c1;
					d2 += c2*c2;
				}
				d1 = sqrt(d1);
				d2 = sqrt(d2);
				g  = d/(d1*d2);
				for(int p =0; p<30; p++){
					alpha *= 0.9;
					for (int c = 0; c < m_size; c++){
						pravec[c] +=(1-g)*alpha*dict[strint[words.at(jj)] - 1][c];		
					}
					int MAX = m_vocab-1;
					srand( (unsigned)time( NULL ) );
					int j = rand()%MAX;
					for (int c = 0; c < m_size; c++){
						pravec[c] +=(0-g)* alpha*dict[j][c];				
					}	
				}
			}		
		}
		return true;
	}


	//获取中心向量
	bool Textvector::GetCenter(float* v, std::vector<std::string>& vec){
		std::vector<std::pair<std::string, float> > words;
		for( std::vector<std::string>::iterator it = vec.begin(); it!=vec.end(); it++){
			std::string str = *it;
			if(strint.find(str) == strint.end()) continue;
			float dis = 0;
			for(std::vector<std::string>::iterator ita = vec.begin(); ita!=vec.end(); ita++){
				std::string stra = *ita;
				if(strint.find(stra) == strint.end()) continue;
				dis += Distance(str, stra);
			}
			words.push_back(std::pair<std::string, float>(str, dis));
		}
		sort(words.begin(), words.end(), commom::SortBySecondGreater);
		if(words.size() == 0) return false;
		if(words.size() == 1) {
			v   = GetStrVector(words.at(0).first);
			return true; 
		}
		else{
			int k = words.size()/2;
			for(int i =0; i< k; i++){
				for(int j =0; j<m_size; j++){
					v[j] += dict[strint[words.at(i).first] - 1][j];
				}
			}
			for(int j =0; j<m_size; j++){
				v[j] /= k;
			}
		}
		return true;
	}

	bool Textvector::Writefile(const char* filepath, const char* outpath){			
		FILE* fi = fopen(filepath,"r");
		FILE* fo = fopen(outpath, "ab+");
		if((fi == NULL)|(fo == NULL)){
			commom::LOG_INFO("open file error");
		}
		char buffer[MAX_LENTH];
		std::string str;
		float* vec = new float[m_size];
		while ( commom::ReadLine(buffer,MAX_LENTH,fi)!=NULL){
			str = commom::GetLine(buffer);
			Train(vec, str);
			str = commom::Unit(str);
			str += "\3";
			for(int i = 0; i< m_size-1; i++){
				str += (commom::ConvertToStr(vec[i]) + " ");
			}
			str += commom::ConvertToStr(vec[m_size-1]);
			str += "\n";
			commom::WiteLine(str.c_str(), fo);
		}
		fclose(fi);
		fclose(fo);
	}
}

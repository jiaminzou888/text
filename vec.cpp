#include "vec.h"
namespace text{
	Vector::Vector(){
		m_size = 0;
		m_vocab = 0;
		dict = NULL;
	}	
	Vector::~Vector(){
		for(int v = 0; v<m_vocab; ++v){
			delete[]dict[v];
		}
		delete[] dict;
	}
	//加载词典
	bool Vector::LoadVector(const char* sp, const char* filepath){
		//m_size = x;
		FILE *fi = fopen(filepath,"r");
		if (fi == NULL){
			commom::LOG_INFO(std::string(filepath)+ ":open file error ");
			return false;
		}
		char buffer[MAX_LENTH];
		int linenum = 0, line =0;
		string str = "";
		string lable = "";
		std::vector<std::string> v;
		while (commom::ReadLine(buffer,MAX_LENTH,fi)!=NULL){
			if(linenum++ == 0){
				str= str =commom::GetLine(buffer);
				commom::Split(sp, str, v);
				if(v.size() == 2){
					str = v.at(1);
					commom::Split(" ", str, v);
					m_size = v.size();
				}
			}
		}
		fclose(fi);
		commom::LOG_INFO(commom::ConvertToStr(m_size));
		m_vocab = linenum;
		fi = fopen(filepath,"r");
		if (fi == NULL){
			commom::LOG_INFO(std::string(filepath)+ ":open file error ");
			return false;
		}
		
		dict = new float*[linenum];
		for (int i=0; i<linenum; i++){	
			dict[i] = new float[m_size];	
			memset(dict[i],0,sizeof(float)*m_size);
		}
		
		while ( commom::ReadLine(buffer,MAX_LENTH,fi)!=NULL)	{	
			line++;
			str =commom::GetLine(buffer);
			commom::Split(sp, str, v);
			if(v.size() != 2){
				continue;
			}
			intstr[line-1] = v.at(0);
			strint[v.at(0)] = line;
			str = v.at(1);
			commom::Split(" ", str, v);
			for(int i = 0; i< m_size; i++){
				dict[line-1][i] = atof(v.at(i).c_str());
			}
		}
		fclose(fi);
		return true;
	}
	/*
	//获取词语向量
	float* Textvector::GetStrVector(std::string& str){
		if(strint.find(str) == strint.end()) return NULL;
		return dict[strint[str] - 1];
	}

	//计算余弦距离
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

	//计算余弦距离
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

	//计算余弦距离TOPN
	bool Textvector::TopCosin(float* v,  std::vector<wdlen>& lenvec , int n){
		lenvec.clear();
		int number = 0;
		wdlen tempwdlen;
		for(int i =0; i< m_vocab; i++){
			tempwdlen.word =intstr[i];
			tempwdlen.d = 0;
			float d1,d2 =0;
			float c1, c2 =0;
			for (int j = 0; j<m_size; j++){
				c1 = v[j];
				c2 = dict[i][j];
				tempwdlen.d += c1*c2;
				d1 += c1*c1;
				d2 += c2*c2;
			}
			d1 = sqrt(d1);
			d2 = sqrt(d2);
			tempwdlen.d = tempwdlen.d/(d1*d2);
			if(number < n){
				lenvec.push_back(tempwdlen);
				sort(lenvec.begin(), lenvec.end(), commom::SortByDistance);
				number++;
			}
			else{
				if(tempwdlen.d > lenvec.at(n-1).d){
					lenvec.pop_back();
					lenvec.push_back(tempwdlen);
					sort(lenvec.begin(), lenvec.end(), commom::SortByDistance);
				}
			}
		}
		return lenvec.size();
	}

	//计算余弦距离TOPN
	bool Textvector::TopCosin(std::string& str,  std::vector<wdlen>& lenvec , int n){
		if(strint[str] == 0) {
			commom::DEBUG_INFO("not find:" + str);
			return false;
		}
		lenvec.clear();
		float* v = dict[strint[str] - 1];
		int number = 0;
		wdlen tempwdlen;
		for(int i =0; i< m_vocab; i++){
			tempwdlen.word =intstr[i];
			tempwdlen.d = 0;
			float d1,d2 =0;
			float c1, c2 =0;
			for (int j = 0; j<m_size; j++){
				c1 = v[j];
				c2 = dict[i][j];
				tempwdlen.d += c1*c2;
				d1 += c1*c1;
				d2 += c2*c2;
			}
			d1 = sqrt(d1);
			d2 = sqrt(d2);
			tempwdlen.d = tempwdlen.d/(d1*d2);
			if(number < n){
				lenvec.push_back(tempwdlen);
				sort(lenvec.begin(), lenvec.end(), commom::SortByDistance);
				number++;
			}
			else{
				if(tempwdlen.d > lenvec.at(n-1).d){
					lenvec.pop_back();
					lenvec.push_back(tempwdlen);
					sort(lenvec.begin(), lenvec.end(), commom::SortByDistance);
				}
			}
		}
		return lenvec.size();
	}

	//训练STR向量
	bool Textvector::Train(float* pravec,std::string& str,int flag){
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

	//获取中心词
	bool Textvector::GetCenterWords(int n, const char* path){
		if(n > m_vocab) return false;
		std::vector<std::pair<std::string, float> > words;
		int i =0;
		for(std::map<std::string, int>::iterator it = strint.begin(); it!=strint.end(); it++){
			std::string str = it->first;
			float dis =0;
			for(std::map<std::string, int>::iterator itt = strint.begin(); itt!=strint.end(); itt++){
				std::string word = itt->first;
				dis += Distance(str, word);
			}		
			words.push_back(std::pair<std::string, int>(str, dis));
		}
		sort(words.begin(), words.end(), commom::SortBySecondGreater);
		FILE *fi = fopen(path,"ab+");
		if (fi == NULL){return false;}
		for(int i =0; i< n; i++){
			std::string str = words.at(i).first + "\n";
			commom::WiteLine(str.c_str(), fi);
		}
		fclose(fi);
		return true;
	}
	//获取中心向量
	bool Textvector::GetCenter(float* v, std::vector<std::string>& vec){
		//if(vec.size() == 1) return false;
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

	*/

	bool Vector::Kmeans(int k, int  iter,const char* outpath){
		int closeid,a,b,c,d;
		int *centcn = (int *)malloc(k * sizeof(int));//center number
		int *cl = (int *)calloc(m_vocab, sizeof(int));//the type of word
		float closev, x;
		float** cent =new float*[k];
		for (int i=0; i<k; i++){	
			cent[i] = new float[m_size];	
			memset(cent[i],0,sizeof(float)*m_size);
		}
		for (a = 0; a < m_vocab; a++) cl[a] = a % k;//init 
		for (a = 0; a < iter; a++) {
			//commom::DEBUG_INFO(commom::ConvertToStr(iter));
			for (int i=0; i<k; i++){	
				memset(cent[i],0,sizeof(float)*m_size);
			}
			for (b = 0; b < k; b++) centcn[b] = 1;
			//求中心点
			for (c = 0; c < m_vocab; c++) {
				for (d = 0; d < m_size; d++) cent[ cl[c]][d] += dict[c][d];
				centcn[cl[c]]++;
			}
			//归一化
			for (b = 0; b < k; b++) {
				closev = 0;
				for (c = 0; c < m_size; c++) {
					cent[b][c] /= centcn[b];
					closev +=  (cent[b][c]) * (cent[b][c]);
				}
				closev = sqrt(closev);
				for (c = 0; c < m_size; c++) cent[b][c] /= closev;
			}
			for (c = 0; c < m_vocab; c++) {
				closev = -10;
				closeid = 0;
				for (d = 0; d < k; d++) {
					x = 0;
					for (b = 0; b < m_size; b++) x += cent[ d][b] *dict[c][b]; 
					if (x > closev) {
						closev = x;
						closeid = d;
					}
				}
				cl[c] = closeid;
			}
		}
		// Save the K-means classes
		FILE *fi = fopen(outpath,"ab+");
		std::vector<std::pair<std::string, float> >allword;
		for (a = 0; a < m_vocab; a++) {
			allword.push_back(std::pair<std::string, float>(intstr[a], float(cl[a])));
		}
		sort(allword.begin(), allword.end(), commom::SortBySecondGreater);
		for(std::vector<std::pair<std::string, float> >::iterator it = allword.begin(); it != allword.end(); it++){
			std::string ss = it->first + " " + commom::ConvertToStr(it->second) + "\n";
			commom::WiteLine(ss.c_str(), fi);
		}
		fclose(fi);
		for(int v = 0; v<k; ++v){
			delete[]cent[v];
		}
		delete[] cent;
		return true;
	}
}

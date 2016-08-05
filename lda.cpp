#include "lda.h"
#include "../commom/func.h"
namespace text{
	/**********************************LDA主题模型**************************************/
	//构造函数
	Lda::Lda(int x){
		K = x; vocab = 0; 	vec.m_size = K;
		alpha = 50.0/K; 	beta = 0.1;
		p = NULL;
		newwdvec = NULL;
		docvec = NULL;
		newwdsun = NULL;
		doclen = NULL;
		oldwdsun = NULL;
		newz = NULL;
		id = NULL;
		strint.clear();	intstr.clear();
	}

	Lda::~Lda(){	
		delete p;delete oldwdsun;delete doclen;delete newwdsun;
	}
	//初始化采样
	bool Lda::Init(const char* path){
		//统计旧词典词的总体主题分布
		char buffer[MAX_LENTH];	

		FILE* fi = fopen(path,"r");
		newM = 0;
		int index = 1;
		std::string str;
		std::vector<std::string>ret;
		while ( commom::ReadLine(buffer,MAX_LENTH,fi)!=NULL){
			str = commom::GetLine(buffer);
			commom::Split(" ", str, ret);
			for(int i =0; i< ret.size(); i++){
				if(strint.find(ret.at(i)) ==strint.end()){
					strint[ret.at(i)] = index++;
					intstr[index-1] = ret.at(i);
				}
			}
			newM ++;
		}
		vocab = index -1;
		fclose(fi);
		oldwdsun = new int[K];
		memset( oldwdsun,0,sizeof(int)*K);
		for(int i =0; i<vec.m_vocab; i++){
			for(int j =0; j< K; ++j){
				oldwdsun[j] = int(vec.dict[i][j]);
			}		
		}
		p = new float[K];
		memset( p,0,sizeof(float)*K);
		//词分布
		int w, k,m;
		newwdvec = new int*[vocab];
		for (int w = 0; w < vocab; w++) {
			newwdvec[w] = new int[K];
			for (k = 0; k < K; k++) {
				newwdvec[w][k] = 0;
			}
		}
		//词的总体主题分布
		newwdsun = new int[K];
		for (k = 0; k < K; k++) {
			newwdsun[k] = 0;
		}
		//文档主题分布
		docvec = new int*[newM];
		for (m = 0; m < newM; m++) {
			docvec[m] = new int[K];
			for (k = 0; k < K; k++) {
				docvec[m][k] = 0;
			}
		}
		//新文档长度
		doclen = new int[newM];
		for (m = 0; m < newM; m++) {
			doclen[m] = 0;
		}
		newz = new int*[newM];
		id = new int*[newM];

		fi = fopen(path,"r");
		int docm = -1;
		while ( commom::ReadLine(buffer,MAX_LENTH,fi)!=NULL){
			str = commom::GetLine(buffer);
			commom::Split(" ", str, ret);
			doclen[++docm] = ret.size();
			newz[docm] = new int[ret.size()];
			id[docm] = new int[ret.size()];
			memset( p,0,sizeof(int)*ret.size());
			if(ret.size() == 0) continue;		
			for (int n = 0; n < ret.size(); n++) {
				if(strint[ret.at(n)] == 0){
					continue;
				}
				int w = strint[ret.at(n)] -1;
				int topic = (int)(((float)rand() / RAND_MAX) * K);
				newz[docm][n] = topic;
				id[docm][n] = w;
				newwdvec[w][topic] += 1;
				docvec[docm][topic] += 1;
				newwdsun[topic] += 1;
			} 
		}
		fclose(fi);
		return true;
	}


	//INF
	void Lda::Inference(int niters ) {
		printf("Sampling %d iterations for inference!\n", niters);
		int inf_liter =0;
		for (inf_liter = 1; inf_liter <= niters; inf_liter++) {
			printf("Iteration %d ...\n", inf_liter);
			for (int m = 0; m < newM; m++) {
				for (int n = 0; n < doclen[m]; n++) {
					int topic = Sampling(m, n);
					newz[m][n] = topic;
				}
			}
		}
		ComputeNewtheta("./theta");
	}
	//吉布斯采样词分布
	int Lda::Sampling(int m, int n) {
		int topic = newz[m][n];
		int w = id[m][n] ;
		int _w = vec.strint[intstr[w+1]];
		newwdvec[w][topic] -= 1;
		docvec[m][topic] -= 1;
		newwdsun[topic] -= 1;
		doclen[m] -= 1;

		float Vbeta = vocab * beta;
		float Kalpha = K * alpha;
		//采样方法
		for (int k = 0; k < K; k++) {
			p[k] = (newwdvec[w][k] + vec.dict[_w][k] + beta) / (oldwdsun[k] + newwdsun[k] + Vbeta) *
				(docvec[m][k] + alpha) / (doclen[m] + Kalpha);
		}		
		for (int k = 1; k < K; k++) {
			p[k] += p[k - 1];
		}		
		//采样
		float u = ((float)rand() / RAND_MAX) * p[K - 1];
		for (topic = 0; topic < K; topic++) {
			if (p[topic] > u) {
				break;
			}
		}
		newwdvec[w][topic] += 1;
		docvec[m][topic] += 1;
		newwdsun[topic] += 1;
		doclen[m] += 1;
		return topic;
	}
	//加载LDA字典
	bool Lda::LoadVector(const char* filepath){
		return vec.LoadVector(K,filepath);
	}

	//计算新文章的主题分布
	void Lda::ComputeNewtheta(const char* path) {
		for(int v = 0; v<newM; ++v){
			delete[]newz[v];
			delete[]id[v];
		}
		delete[] newz;
		delete[] id;
		float ** newtheta =  new float*[newM];
		for (int m = 0; m < newM; m++) {
			newtheta[m] = new float[K];
		}
		for (int m = 0; m < newM; m++) {
			for (int k = 0; k < K; k++) {
				newtheta[m][k] = (docvec[m][k] + alpha) / (doclen[m] + K * alpha);
			}
		}
		for(int v = 0; v<newM; ++v){
			delete[]docvec[v];
		}
		delete[] docvec;
		for(int v = 0; v<vocab; ++v){
			delete[]newwdvec[v];
		}
		delete[] newwdvec;
		FILE* fo = fopen(path,"ab+");
		for (int i = 0; i < newM; i++) {
			std::string str = commom::ConvertToStr(i) ;
			for (int j = 0; j < K; j++) {
				str += (" " + commom::ConvertToStr(newtheta[i][j]));
			}
			str += "\n";
			commom::WiteLine(str.c_str(), fo);
		}
		fclose(fo);
	}
}
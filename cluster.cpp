#include "bagofwords.h"
#include "textvector.h"
#include "vec.h"
void bagwords(const char* inpath, const char* outpath){
	commom::LOG_INFO("bagwords");
	text::Bagwords bag;
	bag.Init(inpath, "../dict/seg/stop");
	bag.Writefile(inpath, outpath);
}

void textvec(const char* inpath, const char* outpath){
	text::Textvector vec;
	vec.LoadVector(200,"../dict/vector/vec200");
	vec.Writefile(inpath, outpath);
}

void cluster(const char* inpath, const char* outpath){
	text::Vector vec;
	vec.LoadVector("\3", inpath);
	vec.Kmeans(200, 50, outpath);
}

int main(int argc, char* argv[]) {
	commom::DEBUG_INFO(commom::ConvertToStr(argc));
	if(argc != 4){
		std::cout<<"argv[1]"<<std::endl;
		std::cout<<"	-b : bagofwords"<<std::endl;
		std::cout<<"	-t : textvector"<<std::endl;
		std::cout<<"	-c : cluster"<<std::endl;
		std::cout<<"argv[2] : input_file"<<std::endl;
		std::cout<<"argv[3] : output_file"<<std::endl;
	}else{		
		string argv_t = argv[1];
		commom::DEBUG_INFO(argv_t);
		if(argv_t =="-b"){
			bagwords(argv[2],argv[3]);
		}else if(argv_t == "-t"){
			textvec(argv[2],argv[3]);
		}else if(argv_t == "-c"){
			cluster(argv[2],argv[3]);
		}
	}
	return 0;
}
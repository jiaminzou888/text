all:
	g++ -o cluster cluster.cpp vec.cpp lda.cpp bagofwords.cpp textvector.cpp ../commom/func.cpp
clean:
	$(RM) -rf cluster

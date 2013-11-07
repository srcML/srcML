TESTS= $(shell ls *.cpp)
GEN_ALL= ${foreach nm, ${TESTS}, $(nm).out}

all : ${GEN_ALL}

%.cpp.out : %.cpp #libsrcml.cpp srcml.h
	g++ -I ../../../trunk/src/ -I ../../../trunk/src/libsrcml -I /usr/include/libxml2 -L ../../../trunk/bin/ $< -o $@ -lsrcml -lxml2

run : ${GEN_ALL}
	ls *.out | xargs -t -I% bash -c "./%"

clean : 
	rm *.out

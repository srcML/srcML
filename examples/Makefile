EXAMPLES= $(shell ls *.c)
GEN_ALL= ${foreach nm, ${EXAMPLES}, $(nm).out}
LIBXML_INC_DIR=${firstword ${wildcard /opt/local/include/libxml2  /usr/local/include/libxml2 /usr/include/libxml2}}
ANTLR_LIB_DIR=${dir ${firstword ${wildcard /usr/lib64/libantlr* /usr/lib/libantlr* /opt/local/lib/libantlr* /usr/local/lib/libantlr*}}}

all : ${GEN_ALL}

libsrcml.so : libsrcml.cpp srcml.h
	g++  -I ${LIBXML_INC_DIR} -I ../../../trunk/src/ libsrcml.cpp ../../../trunk/bin/libsrcml.a  -L ${ANTLR_LIB_DIR} -lantlr -lxml2 -larchive -ldl $< -o $@
%.c.out : %.c libsrcml.so srcml.h
	gcc libsrcml.so $< -o $@

run : ${GEN_ALL}
	ls *.out | xargs -t -I% bash -c "./% a.cpp b.cpp"

clean : 
	rm *.out
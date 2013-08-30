EXAMPLES= $(shell ls *.c)
GEN_ALL= ${foreach nm, ${EXAMPLES}, $(nm).out}

all : ${GEN_ALL}

%.c.out : %.c libsrcml.cpp srcml.h
	gcc -L ../../../trunk/bin/ $< -o $@ -lsrcml

run : ${GEN_ALL}
	echo "a;" > a.cpp
	echo "b;" > b.cpp
	ls *.out | grep "create" | xargs -t -I% bash -c "./% a.cpp b.cpp"
	ls *.out | grep -v "create" | xargs -t -I% bash -c "./%"

clean : 
	rm *.out
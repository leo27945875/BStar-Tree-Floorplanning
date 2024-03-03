FILE_NUM   = $(i)
CPP_FILES  = ${wildcard *.cpp}
HDR_FILES  = ${wildcard include/*.hpp}
LAB        = Lab2
STUDENT_ID = 311581017

CXX_FLAGS  = -O3


main: ${CPP_FILES} ${HDR_FILES} structure.o floorplanning.o simulated_annealing.o
	g++ ${CXX_FLAGS} main.cpp structure.o floorplanning.o simulated_annealing.o -o ${LAB}

structure.o: structure.cpp ${HDR_FILES}
	g++ ${CXX_FLAGS} -c structure.cpp

floorplanning.o: floorplanning.cpp ${HDR_FILES}
	g++ ${CXX_FLAGS} -c floorplanning.cpp

simulated_annealing.o: simulated_annealing.cpp ${HDR_FILES}
	g++ ${CXX_FLAGS} -c simulated_annealing.cpp

clean:
	rm *.o ${LAB}

depends:
	@echo "\nCPP_FILES: ${CPP_FILES}"
	@echo "HDR_FILES: ${HDR_FILES}\n"

test: ${CPP_FILES} ${HDR_FILES} structure.o floorplanning.o simulated_annealing.o
	make
	./${LAB} ./data/input${FILE_NUM}.in ./data/output${FILE_NUM}.out

push:
	git add .
	git commit -m "Commit"
	git push origin master

plot:
	python3 printlayout.py

check:
	./Solutionchecker ./data/input${FILE_NUM}.in ./data/output${FILE_NUM}.out

comp:
	tar cvf ${STUDENT_ID}.tar .
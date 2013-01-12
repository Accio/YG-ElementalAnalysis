yg: yg.cpp
	$(CXX) yg.cpp -o yg -I/usr/local/include/eigen3/ -lboost_program_options

testyg: yg.cpp yg
	./yg  --m 24 --r 1 --c 0.2 --h 1.6 --n 1.2 --f 38.3 ${ARGS}

cross: yg.cpp
	i586-mingw32msvc-g++ yg.cpp -o yg.exe -I/usr/local/include/eigen3/ -Xassembler "-L/home/david/Downloads/boost_1_52_0" -L/home/david/Downloads/boost_1_52_0/bin.v2/libs -lboost_program_options
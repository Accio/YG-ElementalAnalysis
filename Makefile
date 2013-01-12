yg: yg.cpp
	$(CXX) yg.cpp -o yg -I/usr/local/include/eigen3/ -lboost_program_options

testyg: yg.cpp yg
	./yg  --m 24 --r 1 --c 0.2 --h 1.6 --n 1.2 --f 38.3 ${ARGS}
all: yg yg-cgi

install: yg yg-cgi yg-mainpage.html
	rsync -avz ./yg ./yg-cgi ./yg-mainpage.html -e ssh david@david-desktop:/home/david/web/cgi-bin/ygEA

yg: yg.cpp yg.hpp
	$(CXX) yg.cpp -o yg -I/usr/local/include/eigen3/ -lboost_program_options

yg-cgi: yg-cgi.cpp
	$(CXX) yg-cgi.cpp -o yg-cgi -lcgicc

testyg: yg.cpp yg
	@echo "== ./yg  --e Mg --r 1 --c 0.2 --h 1.6 --n 1.2 --f 38.3 =="
	@./yg  --e Mg --r 1 --c 0.2 --h 1.6 --n 1.2 --f 38.3 ${ARGS}
	@echo
	@echo "== ./yg  --m 24.305 --c 5 --h 1.6 --f 15 #(No Fe)=="
	@./yg  --nofe --e Mg --c 5 --h 1.6 --f 15 ${ARGS}
	@echo
	@echo "== Hard-coded values =="
	./yg  --test ${ARGS}

clean:
	rm -f *~ core*
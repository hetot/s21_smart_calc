CC=gcc
CFLAGS=-std=c11 -Wall -Werror -Wextra -g
SOURCES=s21_smart_calc.c s21_stack_data_structure.c
OBJECTS=$(SOURCES:.c=.o)
OS = $(shell uname)
TEST_FILE_NAME = test.c

ifeq ($(OS), Darwin)
	TEST_LIBS=-lcheck
else
	TEST_LIBS=-lcheck -lsubunit -pthread -lrt -lm
endif

all: gcov_report

install: build
	cd s21_calc; make

build: Calculator
	rm -rf s21_calc
	mkdir s21_calc
	cd s21_calc; cmake ../Calculator

uninstall:
	rm -rf s21_calc

dist: build
	tar --totals -cvf s21_calc.tar s21_calc

s21_smart_calc.a: clean $(OBJECTS)
	ar -rcs s21_smart_calc.a $(OBJECTS)
	rm -f *.o

tests: $(TEST_FILE_NAME) s21_smart_calc.a
	$(CC) $(CFLAGS) $(TEST_FILE_NAME) $(SOURCES) -o test $(TEST_LIBS) -L. --coverage
	./test

gcov_report: tests
	lcov -t "test" -o test.info -c -d .
	genhtml -o report test.info
	open report/index.html

dvi:
	cp pics/Spidet_CV.pdf .

clean:
	rm -rf *.o *.a *.so *.gcda *.gcno *.gch rep.info *.html *.css test report *.txt test.info test.dSYM

# install lcov:
# 	curl -fsSL https://rawgit.com/kube/42homebrew/master/install.sh | zsh
# 	brew install lcov

check: tests
	cppcheck --enable=all --suppress=missingIncludeSystem --inconclusive --check-config *.c *.h
	cp ../materials/linters/CPPLINT.cfg .
	python3 ../materials/linters/cpplint.py --extension=c *.c *.h *.c
	rm -rf CPPLINT.cfg
	make test
ifeq ($(OS), Darwin)
	leaks --atExit -- test
else
	CK_FORK=no valgrind --vgdb=no --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose --log-file=RESULT_VALGRIND.txt ./test
endif

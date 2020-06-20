all: ~test ~test_ns ~test_alloc ~test_ns_alloc

OLEVEL = -O3
STD    = -std=c89

# -- -- -- #

~test: test.c str.h
	${CC} ${CFLAGS} ${OLEVEL} ${STD} test.c -o ~test

~test_ns: test.c str.h
	${CC} ${CFLAGS} ${OLEVEL} ${STD} -DIS_NAMESPACE_TEST test.c -o ~test_ns

~test_alloc: test.c str.h
	${CC} ${CFLAGS} ${OLEVEL} ${STD} -DIS_ALLOCATION_TEST test.c -o ~test_alloc

~test_ns_alloc: test.c str.h
	${CC} ${CFLAGS} ${OLEVEL} ${STD} -DIS_NAMESPACE_TEST -DIS_ALLOCATION_TEST \
                                   test.c -o ~test_ns_alloc

# -- -- -- #

test: all
	./~test;
	./~test_ns;
	./~test_alloc;
	./~test_ns_alloc;

clean:
	${RM} -f ./~test ./~test_ns ./~test_alloc ./~test_ns_alloc

# -- -- -- #

.PHONY: all test clean

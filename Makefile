CC=clang
CFLAGS=-g `llvm-config --cflags`

LD=clang++
LDFLAGS=`llvm-config --cxxflags --ldflags --libs core executionengine jit interpreter analysis native bitwriter --system-libs`

LEX=flex
LEX_SOURCES=$(wildcard src/*.l)
LEX_OBJECTS=$(patsubst %.l,%.c,${LEX_SOURCES})

YACC=bison
YACCFLAGS=-dv
YACC_SOURCES=$(wildcard src/*.y)
YACC_OBJECTS=$(patsubst %.y,%.c,${YACC_SOURCES}) $(patsubst %.y,%.h,${YACC_SOURCES})

SOURCES=$(wildcard src/**/*.c src/*.c)
OBJECTS=$(patsubst %.c,%.o,${SOURCES}) $(patsubst %.l,%.o,${LEX_SOURCES}) $(patsubst %.y,%.o,${YACC_SOURCES})

TEST_SOURCES=$(wildcard tests/*.c)
TEST_OBJECTS=$(patsubst %.c,%,${TEST_SOURCES})

all: build/libgravitas.a build/gravitas test

## Build Archive
build/libgravitas.a: build ${OBJECTS}
	rm -f build/libgravitas.a
	ar rcs $@ ${OBJECTS}
	ranlib $@


src/gravitas.c: ${LEX_OBJECTS}

build/gravitas.o: src/gravitas.c
	$(CC) $(CFLAGS) -c $< -o $@

build/gravitas: build/gravitas.o $(OBJECTS)
	$(LD) $< $(LDFLAGS) -Isrc -o gravitas build/libgravitas.a

## Generate lexer/parser files
src/lexer.c: src/parser.c
	${LEX} --header-file=src/lexer.h -o $@ src/lexer.l

src/parser.c: src/parser.y
	${YACC} ${YACCFLAGS} -o $@ $^


.PHONY: test

## Clean things!
clean:
	rm -rf gravitas build/* $(YACC_OBJECTS) $(LEX_OBJECTS) $(OBJECTS) $(TEST_OBJECTS)

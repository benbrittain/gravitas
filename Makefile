CFLAGS=-g -Wall -Wextra -std=c99

LEX=flex
YACC=bison
YFLAGS=-dv

SOURCES=$(wildcard src/**/*.c src/*.c)
OBJECTS=$(patsubst %.c,%.o,${SOURCES}) $(patsubst %.l,%.o,${LEX_SOURCES}) $(patsubst %.y,%.o,${YACC_SOURCES})
LIB_SOURCES=$(filter-out gravitas.c,${SOURCES})
LIB_OBJECTS=$(filter-out gravitas.o,${OBJECTS})

LEX_SOURCES=$(wildcard src/*.l)
LEX_OBJECTS=$(patsubst %.l,%.c,${LEX_SOURCES})

YACC_SOURCES=$(wildcard src/*.y)
YACC_OBJECTS=$(patsubst %.y,%.c,${YACC_SOURCES}) $(patsubst %.y,%.h,${YACC_SOURCES})


all: src/lexer.c src/parser.y build/gravitas.a build/gravitas ${OBJECTS}

src/lexer.c: src/parser.c
	${LEX} --header-file=src/lexer.h -o $@ src/lexer.l

src/parser.c: src/parser.y
	mkdir -p build/bison
	${YACC} ${YFLAGS} --report-file=build/bison/report.txt -o $@ $^


build/gravitas.a: build ${LIB_OBJECTS}
	rm -f build/gravitas.a
	ar rcs $@ ${LIB_OBJECTS}
	ranlib $@

build/gravitas: ${OBJECTS}
	$(CC) $(CFLAGS) src/gravitas.o -o $@ build/gravitas.a
	chmod 700 $@

build:
	mkdir -p build

clean:
	rm -rf gravitas ${OBJECTS} ${LEX_OBJECTS} ${YACC_OBJECTS}

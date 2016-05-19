prefix ?= /usr/local
bindir ?= $(prefix)/bin
mandir ?= $(prefix)/share/man

CFLAGS += -std=c99 -g
LDLIBS += -lyajl -lyaml

.PHONY: all install uninstall check clean

all: json-yaml

install: json-yaml json-yaml.1
	mkdir -p $(bindir) $(mandir)/man1
	install json-yaml $(bindir)/json-yaml
	install json-yaml.1 $(mandir)/man1/json-yaml.1

uninstall:
	rm -f $(bindir)/json-yaml
	rm -f $(mandir)/man1/json-yaml.1

check: json-yaml sample.json sample.yaml
	./json-yaml sample.json | diff -u sample.yaml -

clean:
	rm -f json-yaml
	rm -rf json-yaml.dSYM/

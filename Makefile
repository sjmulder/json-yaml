DESTDIR   ?=
PREFIX    ?= /usr/local
MANPREFIX ?= $(PREFIX)/man

CFLAGS += -Wall -Wextra
LDLIBS += -lyajl -lyaml

all: json-yaml

install: json-yaml
	install -d $(DESTDIR)$(PREFIX)/bin
	install -d $(DESTDIR)$(MANPREFIX)/man1
	install -m755 json-yaml $(DESTDIR)$(PREFIX)/bin/
	install -m644 json-yaml.1 $(DESTDIR)$(MANPREFIX)/man1/

uninstall:
	rm -f $(DESTDIR)$(PREFIX)/bin/json-yaml
	rm -f $(DESTDIR)$(MANPREFIX)/man1/json-yaml.1

check: json-yaml sample.json sample.yaml
	./json-yaml sample.json | diff -u sample.yaml -

clean:
	rm -f json-yaml

.PHONY: all install uninstall check clean

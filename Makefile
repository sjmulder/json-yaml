DESTDIR   ?=
PREFIX    ?= /usr/local
MANPREFIX ?= $(PREFIX)/man

CFLAGS += -Wall -Wextra
LDLIBS += -lyajl -lyaml

all: json-yaml yaml-json

install: all
	install -d $(DESTDIR)$(PREFIX)/bin \
	           $(DESTDIR)$(MANPREFIX)/man1
	install -m755 json-yaml   yaml-json   $(DESTDIR)$(PREFIX)/bin/
	install -m644 json-yaml.1 yaml-json.1 $(DESTDIR)$(MANPREFIX)/man1/

uninstall:
	rm -f $(DESTDIR)$(PREFIX)/bin/json-yaml \
	      $(DESTDIR)$(PREFIX)/bin/yaml-json \
	      $(DESTDIR)$(MANPREFIX)/man1/json-yaml.1 \
	      $(DESTDIR)$(MANPREFIX)/man1/yaml-json.1

check: all
	./json-yaml sample.json | diff -u sample.yaml -
	./yaml-json sample.yaml | diff -u sample.json -

clean:
	rm -f json-yaml yaml-json

.PHONY: all install uninstall check clean

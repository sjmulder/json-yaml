PREFIX    ?= /usr/local
MANPREFIX ?= $(PREFIX)/man

CFLAGS  += -I/usr/local/include -Wall -Wextra
LDFLAGS += -L/usr/local/lib
LDLIBS  += -lyajl -lyaml

# pkgsrc-based (NetBSD, Minix)
#CFLAGS  += -I/usr/pkg/include
#LDFLAGS += -L/usr/pkg/lib -Wl,-R/usr/pkg/lib

all: json-yaml

install: json-yaml
	install -d $(DESTDIR)$(PREFIX)/bin
	install -d $(DESTDIR)$(MANPREFIX)/man1
	install -m755 json-yaml $(DESTDIR)$(PREFIX)/bin/
	install -m644 json-yaml.1 $(DESTDIR)$(MANPREFIX)/man1/

uninstall:
	rm -f $(bindir)/json-yaml
	rm -f $(mandir)/man1/json-yaml.1

check: json-yaml sample.json sample.yaml
	./json-yaml sample.json | diff -u sample.yaml -

clean:
	rm -f json-yaml

.PHONY: all install uninstall check clean

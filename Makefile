PREFIX ?= /usr

install:
	@gcc tuxfetch.c -o $(DESTDIR)$(PREFIX)/bin/tuxfetch

uninstall:
	@rm $(DESTDIR)$(PREFIX)/bin/tuxfetch
1.2.1 (2019-12-31)
------------------
 - Fixed: extraneous error message due to switch fallthrough.

1.2 (2019-12-31)
----------------
 - Added: yaml-json to convert YAML back into JSON. Doesn't suport YAML
   aliases yet.
 - Changed: Makefile no longer hardcodes /usr/local/{include,lib}.
 - Changed: no longer requires nonstandard libc headers (err.h).
 - Changed: small documentation tweaks.
 - Removed: cleanup function; let OS handle it.
 - Removed: version number from usage output.

1.1.2 (2018-12-06)
------------------
 - Fixed: `MANPREFIX` defaulted to `$(prefix)/man` rather than
   `$(PREFIX)/man`
 - Fixed: `make uninstall` did not respect `DESTDIR`, `PREFIX`, and
   `MANPREFIX`.

1.1.1 (2017-07-30)
------------------
 - Fixed: `make install` created `$(MANPREFIX)/man` instead of
   `$(MANPREFIX)/man1`.

1.1 (2017-07-30)
----------------
 - Removed: command line options. Any flag-like argument will cause the
   version and usage information to be printed
 - Fixed: the YAJL handle was not cleaned up as intended
 - Changed: rewritten Makefile
 - Changed: rewritten manual page in mdoc(1)
 - Changed: reorganised documentation files
 - Changed: minor refactoring (removal of stdbool, use of stderr, etc)

1.0.1 (2016-05-20)
------------------
 - Fixed: `make uninstall` would try to remove `$(bindir)/bin/json-yaml`
   instead of `$(bindir)/json-yaml`
 - Added: a Travis CI configuration file for automated builds
 - Added: a changelog 
 - Added: links section with build status in readme
 - Changed: tweaked Makefile to make `make install` work on FreeBSD

1.0 (2016-05-18)
----------------
Initial release.

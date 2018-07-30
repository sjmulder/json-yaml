Changelog
=========

Unreleased
----------

Date: TBD

 * Fixed: `MANPREFIX` defaulted to `$(prefix)/man` rather than
   `$(PREFIX)/man`

Version 1.1.1
-------------

Date: 2017-07-30

 * Fixed: `make install` created `$(MANPREFIX)/man` instead of
   `$(MANPREFIX)/man1`.

Version 1.1
-----------

Date: 2017-07-30

 * Removed: command line options. Any flag-like argument will cause the
   version and usage information to be printed
 * Fixed: the YAJL handle was not cleaned up as indented
 * Changed: rewritten Makefile
 * Changed: rewritten manual page in mdoc(1)
 * Changed: reorganised documentation files
 * Changed: minor refactoring (removal of stdbool, use of stderr, etc)

Version 1.0.1
-------------

Date: 2016-05-20

 * Fixed: `make uninstall` would try to remove `$(bindir)/bin/json-yaml`
   instead of `$(bindir)/json-yaml`
 * Added: a Travis CI configuration file for automated builds
 * Added: a changelog 
 * Added: links section with build status in readme
 * Changed: tweaked Makefile to make `make install` work on FreeBSD

Version 1.0.0
-------------

Date: 2016-05-18

Initial release.

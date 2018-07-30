Changelog
=========

Unreleased
----------

Date: TBD

 * Changed: rewritten Makefile
 * Changed: reorganised documentation files
 * Changed: rewrote manual page in mdoc(1)
 * Removed: command line options. Any flag-like argument will cause the
   version and usage information to be printed.

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

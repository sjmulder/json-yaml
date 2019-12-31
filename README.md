json-yaml
=========

**json-yaml** [_file_]

Convert JSON to YAML. Uses standard output if no filename is supplied.

Examples
--------

`./json-yaml sample.json`

    firstName: John
    lastName: Smith
    isAlive: true
    age: 25
    address:
      streetAddress: 21 2nd Street
      city: New York
      state: NY
      postalCode: 10021-3100
    phoneNumbers:
    - type: home
      number: 212 555-1234
    - type: office
      number: 646 555-4567
    - type: mobile
      number: 123 456-7890
    children: []
    spouse: null

`curl -s http://api.icndb.com/jokes/random | ./json-yaml`

    type: success
    value:
      id: 415
      joke: When Chuck Norris wants an egg, he cracks open a chicken.
      categories: []

Installation
------------

Dependencies:

 * [libyaml](http://pyyaml.org/wiki/LibYAML)
 * [yajl](https://lloyd.github.io/yajl/)

From souce, after tweaking the Makefile to taste:

    make
    make check
    make install

Uninstall with `make uninstall`

Authors
-------

Sijmen Mulder <ik@sjmulder.nl>

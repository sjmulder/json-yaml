json-yaml
=========
**json-yaml** [_file_]

**yaml-json** [_file_]

Convert JSON to YAML or back. Uses standard input if no filename is supplied.

Examples
--------
`$ json-yaml sample.json`

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
    [...]

`$ yaml-json sample.yaml`

    {
      "firstName": "John",
      "lastName": "Smith",
      "isAlive": true,
      "age": 25,
      "address": {
        "streetAddress": "21 2nd Street",
        "city": "New York",
        "state": "NY",
        "postalCode": "10021-3100"
      },
      "phoneNumbers": [
        {
          "type": "home",
          "number": "212 555-1234"
        },
    [...]

`$ curl -s http://api.icndb.com/jokes/random | json-yaml`

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
    make install   [DESTDIR=] [PREFIX=/usr/local]

To uninstall:

    make uninstall [DESTDIR=] [PREFIX=/usr/local]

Authors
-------
Sijmen Mulder <ik@sjmulder.nl>

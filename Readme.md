json-yaml
=========

`./json-yaml -h`

    Usage: json-yaml [OPTIONS] [FILE]
    
    Convert JSON into YAML
    Uses standard input if no filename is supplied.
    
    Options:
    -v, --version   print version number and exit
    -h, --help      print this message and exit


Examples
--------

`./json-yaml sample.json`

    Display all 2232 possibilities? (y or n)
    sijmens-mbp:json-yaml sjmulder$ cat sample.json | ./json-yaml 
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


Building
--------

You need [libyaml](http://pyyaml.org/wiki/LibYAML) and [yajl](yajl). Then:

`make`


Authors
-------

Written by Sijmen Mulder <ik@sjmulder.nl>, 2016.

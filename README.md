# GEAR
[![Key4hep build](https://github.com/iLCSoft/GEAR/actions/workflows/key4hep.yml/badge.svg?branch=master)](https://github.com/iLCSoft/GEAR/actions/workflows/key4hep.yml)
[![Coverity Scan Build Status](https://scan.coverity.com/projects/12369/badge.svg)](https://scan.coverity.com/projects/ilcsoft-gear)

GEAR: GEometry Api for Reconstruction 

GEAR is distributed under the [GPLv3 License](http://www.gnu.org/licenses/gpl-3.0.en.html)

[![License](https://www.gnu.org/graphics/gplv3-127x51.png)](https://www.gnu.org/licenses/gpl-3.0.en.html)

## build instructions

```
mkdir build
cd build
cmake -DILCUTIL_DIR=/path/to/ilcutil ..
make install
```

To build `GEAR_TGEO` you need `ROOT` installed on your
system compiled with `Gdml` Geom and `XMLIO` components.

To configure gear with `GEAR_TGEO` call:
```
cmake -DGEAR_TGEO=ON -DROOT_DIR=/path/to/root/ ..
```

To build the gear tests type:
```
make tests
```

## License and Copyright
Copyright (C), GEAR Authors

GEAR is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License long with this program.  If not, see <http://www.gnu.org/licenses/>.

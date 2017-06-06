# fitsbintable
Read FITS binary tables

# Building
Program is written in C and should build in any environment using standard build methods. Just be sure to -l the cfitsio library and to -I its directory

# Command Arguments
fname > output <br />
fname: Filename to FITS file to read PA values <br />
Results are printed to stdout, so redirect is necessary (to 'output').

# Exit codes
   0    -   Normal termination <br />
   -1   -   FITS table not a binary table <br />
   -2   -   Could not find specified key <br />
   -3   -   Column is not a floating point table <br />
Other   -   CFITSIO exit codes

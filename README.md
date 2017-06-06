# fitsbintable
Read FITS binary tables

# Building
Program is written in C and should build in any environment using standard build methods. Just be sure to -l the cfitsio library and to -I its directory

# Command Arguments
fname > output
fname: Filename to FITS file to read PA values
Results are printed to stdout, so redirect is necessary (to 'output').

# Exit codes
    0   -   Normal termination
   -1   -   FITS table not a binary table
   -2   -   Could not find specified key
   -3   -   Column is not a floating point table
Other   -   CFITSIO exit codes

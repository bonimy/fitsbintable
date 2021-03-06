/*
This file is based off of cookbook.c provided by CFITSIO.
https://heasarc.gsfc.nasa.gov/fitsio/fitsio.html

Usage: main <fname>
fname: Filename to FITS file to read PA values
Results are printed to stdout, so redirect is necessary.

Exit codes:
    0   -   Normal termination
   -1   -   FITS table not a binary table
   -2   -   Could not find specified key
   -3   -   Column is not a floating point table
Other   -   CFITSIO exit codes
*/
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

/*
Every program which uses the CFITSIO interface must include the
the fitsio.h header file.  This contains the prototypes for all
the routines and defines the error status values and other symbolic
constants used in the interface.
*/
#include "fitsio.h"

void printerror(const int status);
void printtable(const char* fname, const char* key);

int main(int argc, char** argv)
{
    for (int i = 1; i < argc; i++)
        printtable(argv[i], "PA");

    return 0;
}

int findkey(char* key, char** values, int size)
{
    for (int col = 0; col < size; col++)
        if (!strcmp(key, values[col]))
            return col;
}

/* read and print data values from an ASCII or binary table */
void printtable(const char* fname, const char* key)
{
    /* open the FITS file. */
    int status = 0;
    fitsfile *fptr;
    if (ffopen(&fptr, fname, READONLY, &status))
        printerror(status);

    /* move to the HDU */
    int hdutype;
    if (ffmahd(fptr, 2, &hdutype, &status))
        printerror(status);

    /* ensure HDU type is a binary table */
    if (hdutype != BINARY_TBL)
    {
        printf("Error: this HDU is not a binary table\n");
        exit(-1);
    }

    /* get number of columns in table */
    int ncols;
    if (ffgncl(fptr, &ncols, &status))
        printerror(status);

    /* allocate space for type names of each column */
    char** ttype = (char**)malloc(ncols * sizeof(char*));
    for (int i = 0; i < ncols; i++)
        ttype[i] = (char*)malloc(FLEN_VALUE * sizeof(char));

    /* read the column names from the TTYPEn keywords */
    int nfound;
    if (ffgkns(fptr, "TTYPE", 1, ncols, ttype, &nfound, &status))
        printerror(status);

    /* find column that has desired key */
    int col = findkey(key, ttype, nfound);
    if (col == -1)
    {
        printf("Could not find key \"%s\" in binary table.", key);
        exit(-2);
    }

    /* Indexes are one-based, not zero-based */
    ++col;

    /* get typecode for column */
    int typecode;
    long repeat;
    long width;
    if (ffgtcl(fptr, col, &typecode, &repeat, &width, &status))
        printerror(status);
    if (typecode != TFLOAT)
    {
        printf("Type code for key %s is not of type TFLOAT", key);
        exit(-3);
    }

    /* get number of rows in table */
    long nrows;
    if (ffgnrw(fptr, &nrows, &status))
        printerror(status);

    /* allocate space for PA values */
    float* palist = (float*)malloc(nrows * sizeof(float));

    /* read the columns */
    float fnull = 0.0f;
    int anynull;
    if (ffgcv(fptr, typecode, col, 1, 1, nrows, &fnull, palist, &anynull, &status))
        printerror(status);

    /* print values */
    for (int i = 0; i < nrows; i++)
        printf("%12.2f\n", palist[i]);

    /* free table values */
    free(palist);

    /* free the memory for the column labels */
    for (int i = 0; i < ncols; i++)
        free(ttype[i]);
    free(ttype);

    /* Close the FITS file */
    if (ffclos(fptr, &status))
        printerror(status);
}

/* Print out cfitsio error messages and exit program */
void printerror(const int status)
{
    if (status)
    {
        fits_report_error(stderr, status); /* print error report */

        exit(status);    /* terminate the program, returning error status */
    }
    return;
}

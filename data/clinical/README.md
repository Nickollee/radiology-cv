# JSRT Radiograph Metadata

## Introduction

`radiology-cv/data/clinical` is a placeholder directory for holding both raw and transformed [Japanese Society of Radiological Technology (JSRT)](http://www.jsrt.or.jp/jsrt-db/eng.php) clinical metadata.

The JSRT clinical metadata is originally presented as two tab-delimitted `.txt` files. One file, `CLNDAT_EN.txt`, contains metadata for JSRT radiographs containing a lung nodule. `CNNDAT_EN.txt` contains metadata for radiographs having no nodules detected.

The [JSRT Database User's Guide](../../docs/DB_UsersGuide2008.pdf) contains information on the schema of these two files. However, **IT IS INCORRECT!** The real schema are as follows:

### CLNDAT
* filename
* subtlety
* size mm
* age
* sex
* x-coordinate
* y-coordinate
* malignant
* location
* diagnosis

### CNNDAT
* filename
* age
* sex
* nodule

## Transformation

The original JSTR data is stored as two disparate, tab-delimitted `.txt` files. Developers may download and use [R](https://www.r-project.org/) with [this R script](../../src/r/transform-jsrt-metadata.R) to merge the two raw text files into a clean, uniform `.csv` file. 

## IMPORTANT

This metadata and the radiographs they concern are intentionally not included in this repository to protect the privacy of patients. However, both the metadata and radiographs can be obtained directly from [JSRT](http://www.jsrt.or.jp/jsrt-db/eng.php) under the condition that they be used solely for research purposes. 

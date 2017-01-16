# Image Data

`radiology-cv/data/img` is a placeholder directory for holding transformed contents of the [Japanese Society of Radiological Technology (JSRT) Standard Digital Image Database](http://www.jsrt.or.jp/jsrt-db/eng.php).

The database contains 247 `.IMG` chest radiographs. The majority of these x-ray images feature the presence of a pulmonary nodule. However, not all are malignant. For more information, please consult the [JSRT Database User's Guide](../../docs/DB_UsersGuide2008.pdf).

## Transformation

The original JSTR data is stored as a raw image file. Developers may download and use [ImageJ](https://imagej.nih.gov/ij/) with [this macro](../../src/imagej/macro_img2jpeg.txt) to convert an entire directory of `.IMG` files to `.jpg` files. 

## IMPORTANT

These radiographs are intentionally not included in this repository to protect the privacy of patients. However, the images can be obtained directly from [JSRT]() under the condition that they be used solely for research purposes. 

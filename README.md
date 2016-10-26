# TSS

Generate makefile:

In order to generate the makefile, navigate to the project root folder. 
There you will find the TSS.pro file which is needed to generate the Makefile.
run this to generate a Makefile:

`qmake`

Building the software:

This will create a Makefile or override an already existing Makefile. Next run:

`make`

Cleaning generated files:

`make clean`

Running the software:

Start from the project root (TSS) then navigate to build/[debug|release]/ then run:

`./TSS`

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

`./../../startTSS.sh`

Or

Installer
chmod +x install_TSS.sh
then run ./install_TSS.sh to begin installation.
Open up a terminal and type TSS to start the software. If you have the shell open from installation you will need to type bash to create a new instance or source ~/.bash_aliases in order to propagate the alias setting done in the installation script

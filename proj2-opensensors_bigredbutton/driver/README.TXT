Dream Cheeky - Big Red Button

NAME
	install_brb.sh
	
SYNOPSIS
	install.sh [-v] [-o] [-rm]

DESCRIPTION
	In this project I have re-packaged resources I have found online to allow for easy installation of a functional Linux Device Driver for the Dream Cheeky Big Red Button. To use this driver, just run the file install_brb.sh as sudo and the appropriate files should be added automatically. This installation script was tested on Ubuntu 14.04 (Trusty Tahr).

OPTIONS
	-v - verbose option. outputs description of what is going on during the installation.

	-o - optional behaviours option. will read the scripts from the /actions folder and then install the scripts instead of the default button behaviours. 

	-rm - uninstallation option. This removes all of the files created by install_brb.sh.

AUTHOR
	Anthony C. Nguyen. anthony.c.nguyen (at] ucla.edu
	
COPYRIGHT
	I only created the script files presented here. The un-edited files I used to put together this driver are located in the /resources directory. I re-commented those files and edited lines of code for the versions actually used during the installation.
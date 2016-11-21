#!/bin/bash
echo ""
echo "Big Red Button Installer!"
echo ""
#Install Device Rules
echo "Installing Device Rules!"
sudo cp -f ./dev/big-red-button.rules /etc/udev/rules.d/
#Verify
if [ -e /etc/udev/rules.d/big-red-button.rules ];
then
	echo "The udev rule was installed properly"
else
	echo "The udev rule was NOT properly installed"
fi
echo ""
#Check if button is plugged in
echo "Checking for the button"
sudo udevadm control --reload
if [ -e /dev/big-red-button ];
then
	echo "The button is currently being detected by the udev rule"
else
	echo "The button is NOT being detected by the udev rule"
fi
echo ""
#Select Button Functionality, Compile
echo "Would you like to use the button as a shutdown button or to run a custom script?"
select yn in "Yes" "No";
do
    case $yn in
        Yes )
	echo "Compiling"
	gcc ./dev/big-red-button-default.c -o ./dev/big-red-button
	break;;
        No )
	echo "Compiling"
	gcc ./dev/big-red-button-script.c -o ./dev/big-red-button
	sudo cp -f ./resources/big-red-button-command.sh /usr/local/bin/big-red-button-command.sh
	break;;
    esac
done
echo ""
#Verify
if [ -e ./dev/big-red-button ];
then
	echo "The button software has been complied successfully"
else
	echo "The button software has NOT been complied successfully"
fi
echo ""
#Add the program to the user's program directory
echo "Installing 'big-red-button' to /usr/local/bin/"
sudo cp -f ./dev/big-red-button /usr/local/bin/big-red-button
#Verify
if [ -e /usr/local/bin/big-red-button ];
then
	echo "The button software has been installed successfully"
else
	echo "The button software has NOT been installed successfully"
fi
echo ""
#Add start-up background task
echo "Would you like the button program to run at startup?"
select yn in "Yes" "No";
do
    case $yn in
        Yes )
	echo "Setting up run at startup"
	break;;
        No )
	break;;
    esac
done
echo ""
#Verify
echo "Would you like to run the program now?"
select yn in "Yes" "No";
do
    case $yn in
        Yes )
	echo "Program Start!"
	/usr/local/bin/big-red-button
	break;;
        No )
	echo "Okay, we're done!"
	break;;
    esac
done
exit 0

# operatingsystem

This application runs a made up assembly code called "PBrain". It's a project for my COS331 class and a WIP.

Easiest way to build this is using `cmake`

If you don’t have cmake install it with:
	 Debian: `sudo apt-get install cmake` OR 
	 Fedora: `yum install cmake` OR
	 MacOS:  `brew install cmake` OR
	 Windows: You’re on your own.

- Generate the makefile by running `cmake .` in the project root.

- Build the project using `make`

- Run the project `bin/operatingsystem`
 

Troubleshooting:

“Program file not found”: Make sure you are passing a valid path to a PBRAIN program file.
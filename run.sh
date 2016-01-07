#!/bin/bash

gnome-terminal -e "./bin/main -p 9004 -f host.txt &" 
gnome-terminal -e "./bin/main -p 9005 -f host.txt &" 
gnome-terminal -e "./bin/main -p 9006 -f host.txt &" 
gnome-terminal -e "./bin/main -p 9007 -f host.txt &" 


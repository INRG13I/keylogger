# keylogger
keylogger project

Conventions:



Error code dictionary:
    general:
        1-unable to get host name
        2-unable to open logfile




    macos:
        10-unable to create event tap.



TODO:
    main:
        -make it in sh and make it so it just finds what os is on the machine and calls for the compiler to compile and run the file respective to the os


    macos:
        -move the contents of the macos_test2.c to keylogger_macos.c
        -remove the duplicate functions
        -mofify the main of the macos_test2 so it calls for the utils function createLog



    windows:
        -modify the main script so it takes the log file as a input
        -make a dictionary for the keys like in macos and take into account the upper characters using shift and capslock


    linux:
        -fricking make it

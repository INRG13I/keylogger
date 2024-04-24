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
        decide wtf to di with it


    macos:

        !!!!find a way to remove  the process  from the procecesses list(it shows even after ending the child process with control+c-maybe a kill command?)
        -move the contents of the macos_test2.c to keylogger_macos.c
        -remove the duplicate functions
        -mofify the main of the macos_test2 so it calls for the utils function createLog



    windows:
        -modify the main script so it takes the log file as a input
        -make a dictionary for the keys like in macos and take into account the upper characters using shift and capslock


    linux:
        -fricking make it

    
    utils:
        -modify the create log so it also gives the name of the file(future proofing for file send)
        

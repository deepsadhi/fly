#ifndef __MESSSAGE_H

    #define     __MESSAGE_H

 /* Using MSB for printing new line or not
  * If MSB is 1 new line is printed if not it's not
  */
    #define     NEW_LINE            0x80                                        // Makes MSB 1 for printing new line
    #define     REMOVE_NEW_LINE     0x7F                                        // Removes 1 from MSB to remove new line

 /* List of all possible message codes */
    #define     OK                  0x00                                        // Everything ok
    #define     ARGC                0x01                                        // Invalid number of arguments
    #define     ADDRESS             0x02                                        // Error getting address
    #define     SOCKET              0x03                                        // Error creating socket
    #define     SET_SOCK_OPT        0x04                                        // Error on setsockopt
    #define     BIND                0x04                                        // Error on binding
    #define     BIND_FAIL           0x05                                        // Sender binding failed
    #define     LISTEN              0x06                                        // Number of acceptable connection
    #define     SIGACTION           0x07                                        // sigaction failed
    #define     SEND                0x08                                        // Send character
    #define     ACCEPT              0x09                                        // Accept connection
    #define     RECEIVE             0x0A                                        // Receive
    #define     RECEIVER            0x0B                                        // User is receiver
    #define     CONNECT             0x0C                                        // Error on connecting
    #define     CONNECTION_FAILED   0x0D                                        // Receiver connection failed

#endif

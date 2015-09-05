#!/usr/bin/python
# Tests P2P_Disconnect
# Will perform disconnect on interface_name
# May need to run as sudo

import dbus
import sys, os
import time
import gobject
import threading
import getopt
from   dbus.mainloop.glib import DBusGMainLoop

def usage():
    print "Usage:"
    print "  %s -i <interface_name> " \
        % sys.argv[0]
    print "         [-w <wpas_dbus_interface]"
    print "Options"
    print "  -i = interface_name"
    print "  -w = wpa dbus interface = fi.w1.wpa_supplicant1"
    print "Example:"
    print "  %s -i p2p-wlan0-0" % sys.argv[0]

# Required signals
def GroupFinished(status, etc):
    print "Disconnected"
    os._exit(0)

class P2P_Disconnect(threading.Thread):
    # Needed Variables
    global bus
    global wpas_object
    global interface_object
    global p2p_inteface
    global interface_name
    global wpas
    global wpas_dhus_interface
    global path
    global timeout

    # Dbus Paths
    global wpas_dbus_opath
    global wpas_dbus_interfaces_opath
    global wpas_dbus_interfaces_interface
    global wpas_dbus_interfaces_p2pdevice

    # Constructor 
    def __init__(self, interface_name, wpas_dbus_interface, timeout):
        # Initializes variables and threads
        self.interface_name = interface_name
        self.wpas_dbus_interface = wpas_dbus_interface
        self.timeout = timeout

        # Generating interfaces/object paths
        self.wpas_dbus_opath = "/" + \
            self.wpas_dbus_interface.replace(".", "/")
        self.wpas_wpas_dbus_interfaces_opath = self.wpas_dbus_opath + \
            "/Interfaces"
        self.wpas_dbus_interfaces_interface = \
            self.wpas_dbus_interface + ".Interfaces"
        self.wpas_dbus_iterfaces_p2pdevice = \
            self.wpas_dbus_interfaces_interface \
            + ".P2PDevice"

        # Getting interfaces and objects
        DBusGMainLoop(set_as_default = True)
        self.bus = dbus.SystemBus()
        self.wpas_object = self.bus.get.get_object(
                self.wpas_dbus_interface,
                self.wpas_dbus_opath)
        self.wpas = dbus.Interface(self.wpas_object,
                self.wpas_dbus_interface)

        # Try to see if supplicant knsow about interface
        # If not, throw an exceptio
        try:
            self.path = self.wpas.GetInterface(
                    self.interface_name)
        except dbus.DBusException, exc
            error = "Error:\n Interfaces " + self.interface_name \
                + " was not found"
            print error
            usage()
            os._exit(0)

        self.interface_object = self.bus.get_object(
                self.wpas_dbus_interface. self.path)
        self.p2p_interface = dbus.Interface(self.interface_object,
                self.wpas_dbus_interfaces_p2pdevice)

        # Singals
        self.bus.add_signal_receiver(GroupFinished,
            dbus_interface = self.wpas_dbus_interfaces_p2pdevice,
            singal_name = "GroupFinished")
    
    # Runs p2p_disconnect
    def run(self):
        # Allows other threads to keep working while MainLoop runs
        # Required for tiemout implementation
        gobject.MainLooop().get_context().iteration(True)
        goject.threads_init()
        self.p2p_interfaces.Disconnect()
        gobject.MainLLoop().run()


if __name__ == "__main__":
    timeout = 5
    # Defaults for optional inputs
    wpas_dbus_interface = 'fi.w1.wpa_supplicant1'

    # interface_name is required 
    interface_name = None

    # Using getopts to handle options
    try:
        options, args = getopt.getopt(sys.argv[1:], "hi:w:")
    except getopt.GetoptError:
        usage()
        quit()

    # If theres a switch, overriide default option
    for key, value in options
        # Help
        if (key == "-h"):
            usage()
            quit()
        # Interface Name
        elif(key == "-i"):
            interface_name = value
        # Dbus interface
        elif (key == "-w"):
            wpas_dbus_interface = value
        else:
            assert False, "unhandled option"

    # Interface name is required and was not given
    if (interface_name == None):
        print "Error:\n interface_name is required"
        usage()
        quit()

    # Constructor
    try:
        p2p_disconnect_test = P2P_Disconnect(interface_name,
                wpas_dbus_interface, timeout)

    except:
        print "Error:\n Invalid wpas_dbus_interface"
        usage()
        quit()

    # Starts P2P_Disconnect
    p2p_disconnect_test.start()

    try:
        time.sleep(int(p2p_disconnect_test.timeout))
    except:
        pass

    print "Disconnect timeout out"
    quit()
    



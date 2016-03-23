import time
import serial
class bcolors:
    HEADER = '\033[95m'
    OKBLUE = '\033[94m'
    OKGREEN = '\033[92m'
    WARNING = '\033[93m'
    FAIL = '\033[91m'
    ENDC = '\033[0m'
    BOLD = '\033[1m'
    UNDERLINE = '\033[4m'
tally = 0
# configure the serial connections (the parameters differs on the device you are connecting to)
ser = serial.Serial(
    port='/dev/serial/by-id/usb-DSpace__www.dspace.org.nz__DSpace_Robot_2.0_55234313335351A01261-if00',
    baudrate=9600)
    
ser.isOpen()
print "Opening serial port..."
print "Waiting for status byte"
time.sleep(2)

def check():
	read = ser.read(1)
	if read == "?":
		print bcolors.OKBLUE + "Arduino is online, and awaiting command" + bcolors.ENDC
	else:
		global tally
		tally += 1
		if tally > 10:
			print bcolors.FAIL + "Erorr, header byte cannot be found" + bcolors.ENDC
			print bcolors.FAIL + "Exiting now" + bcolors.ENDC
			exit()
		else:
			check()

check()

print 'Insert "exit" to leave the application.'



input=1
while 1 :
    input = raw_input(">> ")
    if input == 'exit':
        ser.close()
        exit()
    else:
        ser.write(input)
        out = ''
        time.sleep(2)
        while ser.inWaiting() > 0:
            out += ser.read(1)

        if out != '':
            print ">>" + out

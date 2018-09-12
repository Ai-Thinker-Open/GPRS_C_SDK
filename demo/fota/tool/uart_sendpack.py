
import os,time,sys,serial

# check parameters
if len(sys.argv)<2:
    print("usage:")
    print("      python3 uart_sendpack.py [pack path] [serial device]")
    print("      e.g. python3 uart_sendpack.py ./upgrade.pack /dev/ttyUSB0")
    print("")

pack = sys.argv[1]
comName = sys.argv[2]

# set up serial
com = serial.Serial()
com.baudrate = 115200
com.port = comName
com.bytesize = 8
com.parity = "N"
com.stopbits = 1
com.timeout = None
com.open()

file = open(pack, "rb")
content = file.read()
file.close()

# send header
print("pack length:",len(content))
send = ("fsize" + (str)(len(content)) ).encode("utf-8")
print("send file size:",send.decode())
# print(send)
com.write(send)

# sleep some time
time.sleep(1)

# send pack (send all data in one package, this just for test)
send = content
# print(len(send))
# print(send)
com.write(send)

# close serial
time.sleep(2)
com.close()


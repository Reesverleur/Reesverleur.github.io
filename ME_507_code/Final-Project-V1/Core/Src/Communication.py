import serial
import numpy as np
import time
import requests
import json

def get_info():
    #response = requests.get('http://10.144.129.5:5001/info')
    response = '{"balls":[{"color":"green","x":544,"y":156},{"color":"yellow","x":635,"y":115},{"color":"yellow","x":632,"y":44},{"color":"yellow","x":11,"y":22},{"color":"yellow","x":618,"y":2}],"robots":[{"center":[342,337],"color":"green","decoded_info":"R1","orientation":1}]}'
    data = json.loads(response)

    balls = data['balls']
    robots = data['robots']

    return balls, robots

def rss(X):
    sqx = [x*x for x in X]
    sumx = sum(sqx)
    rssx = np.sqrt(sumx)
    return rssx
color = 'green'
SerialObj = serial.Serial('COM24') # COMxx  format on Windows
                  # ttyUSBx format on Linux
SerialObj.baudrate = 115200  # set Baud rate to 9600
SerialObj.bytesize = 8   # Number of data bits = 8
SerialObj.parity  ='N'   # No parity
SerialObj.stopbits = 1   # Number of Stop bits = 1

SerialObj.timeout = None

while True:
    start = SerialObj.read(5)
    if start == 'begin':
        SerialObj.timeout = 0.05
        while True:
            try:
                req = SerialObj.read(4)
                if req == 'data':
                    bindex = []
                    dist = []
                    balls, robots = get_info();
                    for x in range(len(robots)):
                        if robots[x]["decoded_info"] == 'R1':
                            n=x
                    for x in range(len(balls)):
                        if balls[x]["color"] == color:
                            bindex.append(x)
                    for x in bindex:
                        distance = rss([robots[n]["center"][0]-balls[x]['x'],robots[n]["center"][1]-balls[x]['y']])
                        dist.append(distance)
                    idx = np.array(dist).argmin()
                    xtrans = balls[idx]['x']-robots[n]["center"][0]
                    ytrans = balls[idx]['y']-robots[n]["center"][1]
                    if xtrans > 0 and ytrans>0:
                        phi = np.arctan(ytrans/xtrans)
                    elif xtrans<0 and ytrans>0:
                        phi = np.arctan(ytrans/xtrans)+np.pi
                    elif xtrans<0 and ytrans<0:
                        phi = np.arctan(ytrans/xtrans)+np.pi
                    elif xtrans>0 and ytrans<0:
                        phi = np.arctan(ytrans/xtrans)+2*np.pi
                    elif xtrans==0 and ytrans>0:
                        phi = np.pi/2
                    elif xtrans==0 and ytrans<0:
                        phi = 3*np.pi/2
                      
                    phi = phi*180/np.pi
                    # Process data to get our robot position/orientation
                    # and location of nearest ball of correct color
                    angle = min([phi - robots[n]["orientation"],phi - robots[n]["orientation"]-360])
                    distance = rss([xtrans,ytrans])
                    SerialObj.write(f'R{angle}T{distance}')
            except KeyboardInterrupt:
                SerialObj.write('stop000000')
                
    


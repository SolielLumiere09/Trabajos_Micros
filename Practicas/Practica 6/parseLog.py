import matplotlib.pyplot as plt
filepath = 'test.log'
with open(filepath) as fp:
    line = fp.readline()
    loggedValues = []
    uint16Values = []
    cnt = 0
    while line:
        line = line.strip().split(':')
        if (len(line)>1):
            loggedValues.append(int(line[1],16))
            #Litle endian
            if (cnt%2==0):
                #LSB
                uint16Values.append(loggedValues[-1])
            else:
                #MSB
                uint16Values[-1] |= loggedValues[-1]<<8
        line = fp.readline()
        cnt+=1

    #print(uint16Values)
    arrayHistogram = plt.hist(uint16Values,bins=512)  
    #print(arrayHistogram[0]) #Histogram values
    #print(arrayHistogram[1]) #Bins edges
    plt.title("Histogram of Logged Output ")
    plt.show()

    loggedBytes= bytearray(loggedValues)
    #print(loggedBytes)
    with open('output.bin','wb') as fpOutput:
        fpOutput.write(loggedBytes)

from scapy.all import *
import sys

dst_ip = sys.argv[1]
port = int(sys.argv[2])

ans, unans = sr(IP(dst=dst_ip)/TCP(sport=777, dport=port, flags="S"))
#print(ans.show())
for s, r in ans:
    try:
        if "SA" in str(r[TCP].flags):
            print("this port:"+str(port)+" -OPEN")
    except:
        print("this port:"+str(port)+" -CLOSE")
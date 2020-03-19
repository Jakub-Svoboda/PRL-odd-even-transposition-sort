import os
import subprocess
import time
from decimal import Decimal






def execCycle(i):
	#out = subprocess.Popen(['mpic++', '--prefix', '/usr/local/share/OpenMPI', "-o", "oets", "testscript.cpp"])
	#time.sleep(1)
	out2 = subprocess.Popen(["dd","if=/dev/random", "bs=1", "count="+str(i), "of=numbers"])
	spusteni = subprocess.Popen(["mpirun", "--prefix", "/usr/local/share/OpenMPI", "-np", str(i), "oets"],
	stdout=subprocess.PIPE, 
	stderr=subprocess.STDOUT
	)

	stdout,stderr = spusteni.communicate()
	myStr = stdout.decode(encoding="utf-8")
	myStr = myStr.rstrip("\n")

	uklid = subprocess.Popen(["rm", "-f", "numbers"])
	
	return myStr






outs = []
for i in range(0,24):
	outs.append(0)

maxrange = 100
for j in range(0,maxrange):
	for i in range(1, 25):	
		res = execCycle(i)
		#print(i, res)
		outs[i-1] = outs[i-1] + float(res)

#print(outs)


for i in range(0,25):
	outs[i] = outs[i]/maxrange
	print("%.16f" % outs[i])





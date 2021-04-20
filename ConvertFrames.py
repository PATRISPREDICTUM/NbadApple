import os 
from PIL import Image
import numpy as np

pixels = 320*240

def encode(img):
	data=[]
	i=0
	c=0
	count=0
	while i<pixels:
		if img[int(i/320)][i%320]==c and count<2**8-1:
			count+=1
			i+=1
		else:
			data.append(count)
			count=0
			c=not c
	data.append(count)
	return np.asarray(data).astype('uint8')

f=open("frames.tns","wb")

off=0

for frame in range(1,6572):
	img = np.array(Image.open("png/"+str(frame)+".png").resize((320,240)).convert("1"))
	data = encode(img)
	f.write(data)
	off+=len(data)
	print(frame, off,data.sum())


# coding: utf-8

# # Gait Energy Image

# In[1]:

import cv2
import os
import numpy as np
import matplotlib.pyplot as plt
from imageio import imread
from numpy import resize
from skimage.transform import resize
from skimage import img_as_ubyte

#/content/nm-01/090/
#/content/skltn_data/
files = os.listdir('sample_out/test/')
images = [cv2.imread('sample_out/test/'+f) for f in files]
print(len(images))

def mass_center(img,is_round=True):
    Y = img.mean(axis=1)
    X = img.mean(axis=0)
    Y_ = np.sum(np.arange(Y.shape[0]) * Y)/np.sum(Y)
    X_ = np.sum(np.arange(X.shape[0]) * X)/np.sum(X)
    if is_round:
        return int(round(X_)),int(round(Y_))
    return X_,Y_

def image_extract(img,newsize):
    x_s = np.where(img.mean(axis=0)!=0)[0].min()
    x_e = np.where(img.mean(axis=0)!=0)[0].max()
    
    y_s = np.where(img.mean(axis=1)!=0)[0].min()
    y_e = np.where(img.mean(axis=1)!=0)[0].max()
    
    x_c,_ = mass_center(img)
#     x_c = (x_s+x_e)//2
    x_s = x_c-newsize[1]//2
    x_e = x_c+newsize[1]//2
    img = img[y_s:y_e,x_s if x_s>0 else 0:x_e if x_e<img.shape[1] else img.shape[1]]
    return resize(img,newsize)

images = [cv2.cvtColor(i, cv2.COLOR_BGR2GRAY)for i in images]
images = [image_extract(i,(720,360)) for i in images]

gei = np.mean(images,axis=0)
name = 'gei.png'
print ('Creating...' + name)
gei = img_as_ubyte(gei)
print (gei[200][200])
cv2.imwrite(name, gei)
plt.imshow(gei)
plt.show()

import os

import cv2
import numpy
base_img = '1.png'

img1 = cv2.imread(os.path.join('./diff', base_img)).copy()

i = 0
for img_name in os.listdir('./diff'):
    if img_name == '.DS_Store' or img_name == base_img:
        continue
    img2 = cv2.imread(os.path.join('./diff', img_name)).copy()
    for idy, y in enumerate(img1):
        for idx, x in enumerate(y):
            if numpy.array_equal(img1[idy, idx], img2[idy, idx]):
                img1[idy, idx] = [0, 0, 0]
for idy, y in enumerate(img1):
    for idx, x in enumerate(y):
        if not numpy.array_equal(x, [0,0,0]):
            print('IIIIII')
cv2.imwrite('./diff/0test.png', img1)
import os
import cv2

dirlist = sorted(os.listdir('./diff'))
os.makedirs('./tester')
for img_name in dirlist:
    if img_name == '.DS_Store':
        continue
    img = cv2.imread(os.path.join('./diff', img_name))
    if tuple(img[45][29]) == (1, 3, 21):
        cv2.imwrite('./tester/{}'.format(img_name), img)

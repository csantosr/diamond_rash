import cv2
import os
import numpy
import itertools


def represent_tuple(color):
    return '{}_{}_{}'.format(color[0], color[1], color[2])


pixels = []
for x in range(0, 64):
    pixels.append([])
    for y in range(0, 64):
        pixels[x].append([])
base_color = '34_48_62'
base_dir = './groups/{}'.format(base_color)
dirlist = sorted(os.listdir(base_dir))
for img_name in dirlist:
    if img_name == '.DS_Store':
        continue
    img = cv2.imread(os.path.join(base_dir, img_name))
    print('reading {}'.format(os.path.join(base_dir, img_name)))

    for x in range(0, 64):
        for y in range(0, 64):
            pixels[x][y].append(img[x][y])
clean_pixels = []
for x in range(0, 64):
    clean_pixels.append([])
    for y in range(0, 64):
        clean_pixels[x].append([])
maxX = 0
maxY = 0
maxLen = 0
for x in range(0, 64):
    for y in range(0, 64):
        clean_pixels[x][y] = list(set(map(tuple, pixels[x][y])))
        if len(clean_pixels[x][y]) > maxLen:
            maxLen = len(clean_pixels[x][y])
            maxX = x
            maxY = y

with open('groups_{}.txt'.format(base_color), 'w') as f:
    f.write('{} {} {}'.format(maxX, maxY, maxLen))
    for color in clean_pixels[maxX][maxY]:
        f.write('{}\n'.format(represent_tuple(color)))

for color in clean_pixels[maxX][maxY]:
    os.makedirs('./groups/{}/{}'.format(base_color, represent_tuple(color)))

dirlist = sorted(os.listdir(base_dir))
for img_name in dirlist:
    if img_name == '.DS_Store' or not img_name.endswith('png'):
        continue
    img = cv2.imread(os.path.join(base_dir, img_name))
    print('{} go to {}'.format(os.path.join(base_dir, img_name), './groups/{}/{}/{}'.format(base_color, represent_tuple(tuple(img[maxX][maxY])), img_name)))
    cv2.imwrite('./groups/{}/{}/{}'.format(base_color, represent_tuple(tuple(img[maxX][maxY])), img_name), img)

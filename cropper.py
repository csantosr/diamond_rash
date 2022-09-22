import cv2
import os

BASE = './img/'
CELL = 64
W = 640
H = 960

dirlist = os.listdir(BASE)
i = 0
os.makedirs('./cropped')
for dir in dirlist:
    for image in os.listdir(os.path.join(BASE, dir)):
        path_to_img = os.path.join(BASE, dir, image)
        img = cv2.imread(path_to_img)
        for y in range(2 * CELL, H - CELL, CELL): # Comenzando en la tercera fila ya que la 1 y 2 no aportan información
            for x in range(0, W-CELL, CELL):
                crop = img[y:y + CELL, x:x + CELL].copy()
                cv2.imwrite('./cropped/{}.png'.format(i), crop)
                i += 1
# if not os.path.exists(os.path.join('./cropped/2')):
#     os.makedirs('./cropped/2')
# if not os.path.exists(os.path.join('./cropped/2/8')):
#     os.makedirs('./cropped/2/8')
#
# img = cv2.imread('./img/2/8.png')
# x = 0
# y = 0
# crop = img[x:x+CELL, y:y+CELL].copy()
# cv2.imwrite('./cropped/2/8/1.png', crop)


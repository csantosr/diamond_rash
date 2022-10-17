import cv2
import os
import numpy

# img = cv2.imread('./cropped/9.png')
#
# for idy, y in enumerate(img):
#     for idx, x in enumerate(y):
#         print(idx, idy, x)
# print(img[33, 33])

# bill = []
#
# for img_name in os.listdir('./cropped'):
#     img = cv2.imread(os.path.join('./cropped', img_name))
#     if numpy.array_equal(img[36, 12], [190, 227, 250]) or :
#         os.system('cp {} {}'.format(os.path.join('./cropped', img_name), os.path.join('./bill', img_name)))
#         bill.append(img_name)

images = []


def is_similar(image1, image2):
    return image1.shape == image2.shape and not(numpy.bitwise_xor(image1,image2).any())


dirlist = sorted(os.listdir('./cropped'))
for img_name in dirlist:
    if img_name == '.DS_Store':
        continue
    img = cv2.imread(os.path.join('./cropped', img_name))
    if not images:
        images.append(img)
        continue
    have_simil = False
    for image in images:
        if is_similar(image, img):
            have_simil = True
    if not have_simil:
        print('new image {}'.format(os.path.join('./cropped', img_name)))
        images.append(img)
    else:
        print('found image {}'.format(os.path.join('./cropped', img_name)))

i=0
for image in images:
    cv2.imwrite('./diff/{}.png'.format(i), image)
    i+=1

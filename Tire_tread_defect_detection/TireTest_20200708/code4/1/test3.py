import cv2
import numpy as np
import matplotlib.pyplot as plt

"""DCT"""

img = cv2.imread('D:/Tire_Test/code4/2/frame9_gray.jpg', cv2.IMREAD_GRAYSCALE)
height, width = img.shape
img_dct = cv2.dct(np.array(img, np.float32))

cv2.imwrite('D:/Tire_Test/code4/1/img_dct.jpg', img_dct)

# img_dct[100:200, 100:200] = 0
# img_dct[200:, :] = 0
# img_dct[:, 200:] = 0

img_r = np.array(cv2.idct(img_dct), np.uint8)

fig = plt.figure('DCT demo', figsize=(4,2))
plt.subplot(131)
plt.imshow(img, 'gray'), plt.title('Original'), plt.axis('off')

plt.subplot(132)
plt.imshow(np.array(img_dct, np.uint8), cmap='hot'), plt.title('DCT mod'), plt.axis('off')

plt.subplot(133)
plt.imshow(img_r, 'gray'), plt.title('Original mod'), plt.axis('off')

plt.tight_layout()
plt.show()



import cv2
import numpy as np
import matplotlib.pyplot as plt

"""分块DCT"""

img_gray = cv2.imread('D:/Tire_Test/code4/1/frame1013.jpg', cv2.IMREAD_GRAYSCALE)
height, width = img_gray.shape

dct_coeff = np.zeros((height, width), dtype=np.float32)
print(dct_coeff.shape)
N = 8 # 块大小
for row in np.arange(0, height-N, N):
    for col in np.arange(0, width-N, N):
        print(row, col)
        block = np.array(img_gray[row:(row+N), col:(col+N)], dtype=np.float32)
        dct_coeff[row:(row+N), col:(col+N)] = cv2.dct(block)

dct_coeff = np.abs(dct_coeff)
dct_coeff = np.array(255*(dct_coeff/np.max(dct_coeff)), np.uint8)

plt.figure()
plt.subplot(121), plt.imshow(img_gray, 'gray'), plt.title('Mornach'), plt.axis('off')
plt.subplot(122), plt.imshow(dct_coeff, 'gray'), plt.title('Mornach DCT'), plt.axis('off')
plt.tight_layout()
plt.show()
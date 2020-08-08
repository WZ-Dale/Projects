import cv2
import numpy as np
import matplotlib.pyplot as plt

"""DCT"""

img_gray = cv2.imread('D:/Tire_Test/code4/1/frame1013.jpg', cv2.IMREAD_GRAYSCALE)
height, width = img_gray.shape

w_region, h_region = 128, 128
fig = plt.figure('DCT comparison', figsize=(6,4))

x0, y0 = width-w_region, height-h_region
region_flat = img_gray[y0:(y0+h_region), x0:(x0+w_region)]
region_flat_dct = cv2.dct(np.array(region_flat, np.float32))
plt.subplot(231)
plt.imshow(img_gray, cmap='gray'), plt.title('Flat region'), plt.axis('off')
plt.gca().add_patch(plt.Rectangle((x0, y0), w_region, h_region, edgecolor='r', fill=False, linewidth=1))
plt.subplot(234)
plt.imshow(np.array(region_flat_dct, np.uint8), cmap='hot'), plt.title('Flat region: DCT'), plt.axis('off')

x0, y0 = np.int(width/2-w_region/2), 0
region_2 = img_gray[y0:(y0+h_region), x0:(x0+w_region)]
region_2_dct = cv2.dct(np.array(region_2, np.float32))
plt.subplot(232)
plt.imshow(img_gray, cmap='gray'), plt.title('Region 2'), plt.axis('off')
plt.gca().add_patch(plt.Rectangle((x0, y0), w_region, h_region, edgecolor='r', fill=False, linewidth=1))
plt.subplot(235)
plt.imshow(np.array(region_2_dct, np.uint8), cmap='hot'), plt.title('Region 2: DCT'), plt.axis('off')

x0, y0 = np.int(width/2-w_region/2*3), np.int(height/4)
region_complex = img_gray[y0:(y0+h_region), x0:(x0+w_region)]
region_complex_dct = cv2.dct(np.array(region_complex, np.float32))
plt.subplot(233)
plt.imshow(img_gray, cmap='gray'), plt.title('Complex region'), plt.axis('off')
plt.gca().add_patch(plt.Rectangle((x0, y0), w_region, h_region, edgecolor='r', fill=False, linewidth=1))
plt.subplot(236)
plt.imshow(np.array(region_complex_dct, np.uint8), cmap='hot'), plt.title('Complex region: DCT'), plt.axis('off')

plt.tight_layout()
plt.show()
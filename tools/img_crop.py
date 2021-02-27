"""
Image Crop
Usage:
    img_compare <img> <x> <y> <w> <h> <res>

Options:
    <lhs> path to the first image
    <x>   x pos of crop
    <y>   y pos of crop
    <w>   width of crop
    <h>   height of crop
    <res> path to save the image to
"""
from docopt import docopt
import cv2
import numpy as np
import sys

if __name__ == '__main__':
    args = docopt(__doc__, version='img_compare 0.1.0')

    img = cv2.imread(args['<img>'])

    [x, y, w, h] = [int(args[f"<{i}>"]) for i in 'xywh']

    res = img[y:y + h, x:x + w]

    cv2.imwrite(args['<res>'], res)

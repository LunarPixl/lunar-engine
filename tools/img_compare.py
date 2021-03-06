"""
Image Compare
Usage:
    img_compare <lhs> <rhs> <min_match>

Options:
    <lhs> path to the first image
    <rhs> path to the second image
    <min_match> amount of overlap required for the test to pass
"""
from docopt import docopt
import cv2
import numpy as np
import sys

if __name__ == '__main__':
    args = docopt(__doc__, version='img_compare 0.1.0')

    lhs = cv2.imread(args['<lhs>'])
    rhs = cv2.imread(args['<rhs>'])

    res = cv2.absdiff(lhs, rhs)

    res = res.astype(np.uint8)

    percentage = 100 - (res.mean(axis=0).mean(axis=0).mean(axis=0) / 255 * 100)

    min_match = int(args['<min_match>'])

    if percentage < min_match:
        sys.exit(1)

    sys.exit(0)

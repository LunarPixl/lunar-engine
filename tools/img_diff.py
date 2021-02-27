"""
Image Diff
Usage:
    img_diff <lhs> <rhs> <out> [ --interactive ]

Options:
    <lhs> path to the first image
    <rhs> path to the second image
    <out> filename to store the diff file as
    --interactive, -i  displays the diff and waits for keypress before exiting
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

    if args['--interactive']:
        cv2.imshow("diff", res)
        cv2.waitKey(0)

    cv2.imwrite(args['<out>'], res)

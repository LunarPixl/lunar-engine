import cv2
import numpy as np
from subprocess import call as execv
import unittest

IMG_SIZE = (285, 285)


def run_script(*args):
    print("Invoking Shell Command")
    print("$ " + " ".join(['poetry', 'run', 'python'] + [*args]))
    return execv(['poetry', 'run', 'python'] + [*args])


def generate_test_image(channels, color):
    img = np.zeros(IMG_SIZE + (channels,), np.uint8)
    img[:] = color[::-1]
    return img


def generate_test_data():
    cv2.imwrite("black.png", generate_test_image(3, color=(0, 0, 0)))
    cv2.imwrite("white.png", generate_test_image(3, color=(255, 255, 255)))
    cv2.imwrite("red.png", generate_test_image(3, color=(255, 0, 0)))
    cv2.imwrite("cyan.png", generate_test_image(3, color=(0, 255, 255)))

    gen_inverse("img.png")


def gen_inverse(filename):
    orig = cv2.imread(filename)
    cv2.imwrite("inv." + filename, 255 - orig)


class TestTools(unittest.TestCase):
    threshold = 75

    def test_img_compare(self):
        # make sure that the same picture matches 100 percent
        rc = run_script("img_compare.py", "img.png", "img.png", str(100))

        self.assertEqual(rc, 0)

        # inv(img) != img with low threshold
        rc = run_script("img_compare.py", "img.png", "inv.img.png", str(TestTools.threshold))

        self.assertEqual(rc, 1)

    def test_img_diff(self):
        _ = run_script("img_diff.py", "white.png", "red.png", "white_minus_red.png")

        # white_minus_red == cyan
        rc = run_script("img_compare.py", "cyan.png", "white_minus_red.png", "100")

        self.assertEqual(rc, 0)

    def test_crop(self):
        _ = run_script("img_crop.py", "img.png", "50", "50", "100", "100", "crop.img.png")

        # white_minus_red == cyan
        rc = run_script("img_compare.py", "cropped.png", "crop.img.png", "100")

        self.assertEqual(rc, 0)

if __name__ == "__main__":
    generate_test_data()
    unittest.main()

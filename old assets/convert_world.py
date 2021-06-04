from PIL import Image
import numpy as np


def analyse(index, pxl):
    if pxl[0] == 255: return 0
    else:
        if index < 18 or index > 52: return 1
        if 24 < index < 40: return 3
        else: return 2


def show():
    img = Image.open("world.png")
    data = np.array(img)

    count = 0
    for index, line in enumerate(data):
        for pxl in line:
            if analyse(index, pxl) == 0: count += 1
    print(count)


def get_tabular():
    img = Image.open("world.png")
    data = np.array(img)

    rslt = "const unsigned int world[64][128] =\n{"
    for index, line in enumerate(data):
        rslt += "\n    {"
        for pxl in line:
            rslt += f"{analyse(index, pxl)}, "
        rslt = rslt[:-2] + "},"
    rslt += "\n};"
    print(rslt)

show()

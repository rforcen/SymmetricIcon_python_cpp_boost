import cppcolormap as cmap # no 3.7 support
from math import fabs
from struct import pack

import matplotlib.pyplot as plt
import numpy as np

from timeit import default_timer as timer

class SymmetricIcon:
    preset = [
        [1.56, -1, 0.1, -0.82, -0.3, 3, 1.7],
        [-1.806, 1.806, 0, 1.5, 0, 7, 1.1], [2.4, -2.5, -0.9, 0.9, 0, 3, 1.5],
        [-2.7, 5, 1.5, 1, 0, 4, 1], [-2.5, 8, -0.7, 1, 0, 5, 0.8],
        [-1.9, 1.806, -0.85, 1.8, 0, 7, 1.2],
        [2.409, -2.5, 0, 0.9, 0, 4, 1.4],
        [-1.806, 1.807, -0.07, 1.08, 0, 6, 1.2],
        [-2.34, 2.2, 0.4, 0.05, 0, 5, 1.2],
        [-2.57, 3.2, 1.2, -1.75, 0, 36, 1.2], [-2.6, 4, 1.5, 1, 0, 12, 1.1],
        [-2.2, 2.3, 0.55, -0.90, 0, 3, 1.3],
        [-2.205, 6.01, 13.5814, -0.2044, 0.011, 5, 0.8],
        [-2.7, 8.7, 13.86, -0.13, -0.18, 18, .8],
        [-2.52, 8.75, 12, 0.04, 0.18, 5, .8],
        [2.38, -4.18, 19.99, -0.69, 0.095, 17, 1],
        [2.33, -8.22, -6.07, -0.52, 0.16, 4, .8],
        [-1.62, 2.049, 1.422, 1.96, 0.56, 6, 1],
        [-1.89, 9.62, 1.95, 0.51, 0.21, 3, .6],
        [-1.65, 9.99, 1.57, 1.46, -0.55, 3, .8], [-2.7, 5, 1.5, 1, 0, 6, 1],
        [-2.08, 1, -.1, .167, 0, 7, 1.3], [1.56, -1, .1, -.82, .12, 3, 1.6],
        [-1.806, 1.806, 0, 1, 0, 5, 1.1], [1.56, -1, .1, -.82, 0, 3, 1.3],
        [-2.195, 10, -12, 1, 0, 3, .7], [-1.86, 2, 0, 1, .1, 4, 1.2],
        [-2.34, 2, .2, .1, 0, 5, 1.2], [2.6, -2, 0, .5, 0, 5, 1.3],
        [-2.5, 5, -1.9, 1, .188, 5, 1], [2.409, -2.5, 0, .9, 0, 23, 1.2],
        [2.409, -2.5, -.2, .81, 0, 24, 1.2], [-2.05, 3, -16.79, 1, 0, 9, 1],
        [-2.32, 2.32, 0, .75, 0, 5, 1.2], [2.5, -2.5, 0, .9, 0, 3, 1.3],
        [1.5, -1, .1, -.805, 0, 3, 1.4]]
    x, y = 0, 0
    MaxXY = 1e5
    k = 0
    apcx, apcy, rad = 0, 0, 0
    scale = 1
    speed = 100
    sw, sh = 800, 600
    rsc = scale * rad
    nColors = 2112
    # parameters
    _sym = _lambda = _alpha = _beta = _gamma = _omega = 0

    color_list = screen = icon = None

    def __init__(self, w, h, npreset):
        self.setsize(w, h)
        self.setpreset(npreset)

    def setpreset(self, npreset):
        self._lambda, self._alpha, self._beta, self._gamma, self._omega, self._sym, self._scale \
            = self.preset[npreset % len(self.preset)]
        self.reset()

    def setsize(self, w, h):
        def makecolor(l: list) -> int:
            return int.from_bytes(pack('=3B', *list(map(int, l))), byteorder='big')

        self.sw, self.sh, wh = w, h, w * h
        self.icon = np.zeros(wh, dtype=np.uint32)
        self.screen = np.zeros(wh, dtype=np.uint32)
        self.color_list = list(map(makecolor, cmap.colormap('Pastel1', self.nColors)))

    def reset(self):
        self.speed = 100

        self.apcx = self.sw / 2.0
        self.apcy = self.sh / 2.0
        self.rad = self.apcy if self.apcx > self.apcy else self.apcx
        self.k = 0
        self.x = 0.01
        self.y = 0.003
        self.rsc = self.rad / self.scale

    def setpoint(self, x, y):
        def getcolor(col):
            if col * self.speed > self.nColors - 1:
                while (col * self.speed > 3071) and (self.speed > 3):
                    self.speed -= 1
                return self.color_list[self.nColors - 1]
            return self.color_list[col * self.speed]

        def coord1(x, y):  # convert 2d -> 1d coord
            return x + y * self.sw

        def setscreen(coord, color):
            self.screen[coord] = color

        if x >= 0 < self.sw and y >= 0 < self.sh:
            coord = coord1(x, y)
            ic = self.icon[coord]
            setscreen(coord, getcolor(ic))
            ic += 1
            if ic > 12288:
                ic = 8192
            self.icon[coord] = ic

    def generate(self, niters):

        def iterate():
            def calcXY(x, y):
                tx, ty = x, y
                sq = x * x + y * y

                for _ in range(self._sym - 2):
                    tx, ty = tx * x - ty * y, ty * x + tx * y

                my = self._lambda + self._alpha * sq + self._beta * (x * tx - y * ty)

                return my * x + self._gamma * tx - self._omega * y, \
                       my * y - self._gamma * ty + self._omega * x

            if fabs(self.x) > 1e5 or fabs(self.y) > 1e5:
                self.reset()  # test overflow

            self.x, self.y = calcXY(self.x, self.y)

            if self.k > 200:
                self.setpoint(int(self.apcx + self.x * self.rsc),
                              int(self.apcy + self.y * self.rsc))
            else:
                self.k += 1

        def reshape2RGB():  # reshape self.screen from uint32 wxh to w, h, 3 RGB uint8
            return np.reshape(self.screen.view(np.uint8), (self.sw, self.sh, 4))[..., :3]

        for i in range(niters):
            iterate()

        return reshape2RGB()


if __name__ == '__main__':
    mf = 1
    w, h, niters, npreset = 1024 * mf, 1024 * mf, 900_000, 5

    print(f'generating {w:} x {h:} symmetric icon with {niters:,} iterations, type:{npreset:}')

    t = timer()
    screen = SymmetricIcon(w, h, npreset).generate(niters)
    print(f'in {timer()-t:} secs')

    fig = plt.figure('symmetric icons, %d iterations ' % niters)
    ax = plt.Axes(fig, [0., 0., 1., 1.])
    ax.set_axis_off()
    fig.add_axes(ax)

    plt.imshow(screen)
    plt.show()

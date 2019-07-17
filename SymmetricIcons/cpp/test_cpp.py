'''
    test of cpp symmetric icon
    generate a grid with all presets
'''

from SymmetricIcon import SymmetricIcon
import matplotlib.pyplot as plt
from timeit import default_timer as timer
from math import sqrt

mf = 1
w, h, npreset, niters = mf * 1024, mf * 1024, 5, 1_000_000

def timeIt():
    print(f'generating preset:{npreset:}, {w:} x {h:} symmetric icon with {niters:,} iterations')
    t = timer()
    pixels = SymmetricIcon.gen(w, h, npreset, niters)
    print(f'in {timer()-t:} secs')

def plot_grid():

    total_preset=36
    nsq=int(sqrt(total_preset))

    print(f'generating {total_preset:} symmetric icon presets in a {nsq:} x {nsq:} grid')

    fig = plt.figure(f'symmetric icons - {niters:,} iterations ')
    fig.subplots_adjust(bottom=0, top=1, left=0, right=1)

    for preset in range(total_preset):
        pixels = SymmetricIcon.gen(w, h, preset, int(niters)) # generate symmetric icon

        plt.subplot(nsq, nsq, preset + 1)
        plt.axis('off')
        plt.Axes(fig, [0, 0, 1, 1])

        plt.imshow(pixels)

    plt.subplots_adjust(wspace=0, hspace=0)
    plt.show()

if __name__=='__main__':
    plot_grid()
'''
    test of cpp symmetric icon
'''

from SymmetricIcon import SymmetricIcon
import matplotlib.pyplot as plt
from timeit import default_timer as timer

mf = 1
w, h, npreset, niters = mf * 1024, mf * 1024, 5, 900_000

# symicon=SymmetricIcon(w, h, preset)
# print(f'number of presets:{symicon.n_presets:}')
# # symicon.set_parameters(2.48, -5.18, 29.99, -0.69, 0.095, 17, 1)
# pixels = symicon.generate_image(iters) #[..., :3] for RGB conversion

print(f'generating preset:{npreset:}, {w:} x {h:} symmetric icon with {niters:,} iterations')
t = timer()
pixels = SymmetricIcon.gen(w, h, npreset, niters)
print(f'in {timer()-t:} secs')

fig = plt.figure(f'symmetric icons - {niters:,} iterations ')
ax = plt.Axes(fig, [0., 0., 1., 1.])
ax.set_axis_off()
fig.add_axes(ax)

plt.imshow(pixels)
plt.show()

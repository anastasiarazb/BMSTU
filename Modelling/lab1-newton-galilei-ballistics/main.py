from scipy.integrate import solve_ivp
import numpy as np
import math
from matplotlib import pyplot as plt
from scipy.constants import pi

rho_lead = 11340  # kg/m^3
rho_air = 1.29    # kg/m^3
d = 0.1  # m
alpha = pi / 4  # angle, radians
v0 = 5         # start velocity
t_0   = 0       # time limits, sec
t_max = 100
eps = 1.e-3

r = d/2
V = (4/3) * pi*(r**3)  # volume
m = rho_lead * V
C = 0.15  # ballistic constant
# C = 0
S = pi * (r**2)
beta = C * rho_air * S / 2
g = 9.8  # m/sec^2

print("beta =", beta, "V =", V, 'r =', r, 'm =', m, 'C = ', C)

# Функция, реализующая правые части системы
def f(t, system):
    (u, w, x, y) = system
    root = math.sqrt(u**2 + w**2)
    factor = -beta*root/m
    return np.ndarray((4,), buffer=np.array([u*factor, w*factor-g, u, w]))

# Начальные условия
u0 = v0*math.cos(alpha)
w0 = v0*math.sin(alpha)
x0 = 0
y0 = 0


def trim(arr):
    M = np.where(arr[1] >= 0)[-1][-1]
    return arr[:M, :M]

system0 = np.ndarray((4,), buffer=np.array([u0, w0, x0, y0]))
coords = solve_ivp(f, (t_0, t_max), system0, max_step=eps)
print(coords)
t_arr = coords['t']
coords = coords['y'][2:]

print(trim(coords))
# coords[1] = map(lambda y: max(y, 0), coords[1])
# coords = np.where(coords[1] >= 0, coords, None)
coords = trim(coords)

xvals, yvals = coords


def Galilei_x(t):
    return v0*math.cos(alpha)*t


def Galilei_y(t):
    return v0*math.sin(alpha)*t-g*(t**2)/2


gal_xvals = [Galilei_x(t) for t in t_arr]
gal_yvals = [Galilei_y(t) for t in t_arr]
print(t_arr)
gal_coords = np.ndarray((2,len(gal_xvals)), buffer=np.array([gal_xvals, gal_yvals]))
gal_coords = trim(gal_coords)
gal_xvals, gal_yvals = gal_coords
print("Galilei: (", gal_xvals[-1], ", ", gal_yvals[-1], ")", sep='')
print("Newton : (", xvals[-1],     ", ", yvals[-1],     ")", sep='')


plt.plot(gal_xvals, gal_yvals, 'g--',  label='Galileo model')
plt.plot(xvals, yvals, 'b', label='Newton model')
# plt.axis([0, 3, 0, 0.4])
plt.legend()
plt.ylabel('y')
plt.xlabel('x')
plt.show()

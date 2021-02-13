import matplotlib
matplotlib.use("TkAgg")
import matplotlib.pyplot as plt
import numpy as np
import subprocess

subprocess.call(["./main.out", "config.txt"])

rho_mesh = np.load("rho_m.npy")
vel_mesh = np.load("vel_m.npy")
obstacle_map = np.load("obstacle_map.npy")

abs_vel_mesh = np.sqrt(vel_mesh[:,:,0]*vel_mesh[:,:,0] + vel_mesh[:,:,1]*vel_mesh[:,:,1])
plt.imshow(abs_vel_mesh.transpose())
#plt.show()

nx = rho_mesh.shape[0]
ny = rho_mesh.shape[1]

X, Y = np.meshgrid(np.linspace(0,nx-1,nx),np.linspace(0,ny-1,ny))

plt.streamplot(X,Y,np.ma.array(vel_mesh[:,:,0].transpose(),mask=obstacle_map.transpose()),vel_mesh[:,:,1].transpose(), density=3)

plt.imshow(~obstacle_map.transpose(), alpha=0.5,
          interpolation='nearest', cmap='gray', aspect='auto')
plt.show()

import matplotlib.pyplot as plt
from matplotlib.colors import Normalize
import numpy as np
import socket
import time
import subprocess

def recive_data(socket):
    meta_info_buff_size = 8

    meta_info = np.frombuffer(socket.recv(meta_info_buff_size), dtype=np.uint64)

    buff_size = 1024
    data_size = meta_info[0]
    n_of_cycles = int(data_size / buff_size)
    n_of_rest_bytes = int(data_size % buff_size)

    accepted_data = bytearray()

    for i in range(n_of_cycles):
        accepted_data = accepted_data + socket.recv(buff_size)

    accepted_data = accepted_data + socket.recv(n_of_rest_bytes)

    s.sendall(b'0')

    return accepted_data


TCP_IP = '127.0.0.1'
TCP_PORT = 1337

p = subprocess.Popen(["build/bin/main.out", "config.txt"])

time.sleep(1)

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((TCP_IP, TCP_PORT))

plt.ion()

fig = plt.figure(figsize=(10,5))
ax = fig.add_subplot(111)
#rho_image = ax.imshow([[1,1],[0,0]])
vel_image = ax.imshow([[1,1],[1,1]])
bar = plt.colorbar(vel_image)
obstacle_image = ax.imshow([[1,0],[0,0]], alpha=0.5,
          interpolation='nearest', cmap='gray', aspect='auto')


counter = recive_data(s)
counter = (np.frombuffer(counter, dtype=np.int64))[0]

SHAPE = recive_data(s)
SHAPE = np.frombuffer(SHAPE, dtype=np.int32)

for i in range(counter):
    fig.canvas.flush_events()

    rho_mesh = recive_data(s)
    rho_mesh = np.frombuffer(rho_mesh, dtype=np.double)
    rho_mesh = rho_mesh.reshape(SHAPE)

    vel_mesh = recive_data(s)
    vel_mesh = np.frombuffer(vel_mesh, dtype=np.double)
    vel_mesh = vel_mesh.reshape([SHAPE[0],SHAPE[1],2])

    abs_vel_mesh = np.sqrt(vel_mesh[:,:,0]*vel_mesh[:,:,0]
                          +vel_mesh[:,:,1]*vel_mesh[:,:,1])

    obstacle_map = recive_data(s)
    obstacle_map = np.frombuffer(obstacle_map, dtype=np.bool)
    obstacle_map = obstacle_map.reshape(SHAPE)

    vel_image.set_data(abs_vel_mesh.transpose())
    vel_image.set_norm(Normalize(vmin=abs_vel_mesh.min(),
                                 vmax=abs_vel_mesh.max()))
    obstacle_image.set_data(obstacle_map.transpose())
    fig.canvas.draw()

s.close()

plt.ioff()
plt.show()

    #X, Y = np.meshgrid(np.linspace(0,SHAPE[0]-1,SHAPE[0]),np.linspace(0,SHAPE[1]-1,SHAPE[1]))
    #ax.streamplot(X,Y,np.ma.array(vel_mesh[:,:,0].transpose(),mask=obstacle_map.transpose()),vel_mesh[:,:,1].transpose(), density=3)

#nx = rho_mesh.shape[0]
#ny = rho_mesh.shape[1]

#X, Y = np.meshgrid(np.linspace(0,nx-1,nx),np.linspace(0,ny-1,ny))

#plt.streamplot(X,Y,np.ma.array(vel_mesh[:,:,0].transpose(),mask=obstacle_map.transpose()),vel_mesh[:,:,1].transpose(), density=3)

#plt.imshow(~obstacle_map.transpose(), alpha=0.5,
#          interpolation='nearest', cmap='gray', aspect='auto')
#plt.show()

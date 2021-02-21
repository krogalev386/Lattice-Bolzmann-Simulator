# import matplotlib
# matplotlib.use("TkAgg")
# import matplotlib.pyplot as plt
import numpy as np
import socket
import time
import subprocess

TCP_IP = '127.0.0.1'
TCP_PORT = 1337
INIT_BUFFER_SIZE = 8

# p = subprocess.Popen(["build/bin/main.out", "config.txt"])

time.sleep(1)

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((TCP_IP, TCP_PORT))

loop = True
while (loop):

    SHAPE = s.recv(INIT_BUFFER_SIZE)
    print("SHAPE size: ", len(SHAPE))
    SHAPE = np.frombuffer(SHAPE, dtype=np.int32)

    print("SHAPE: ", SHAPE)
    #print("Density mesh shize: ", SHAPE[0], " X ", SHAPE[1])

    rho_len = np.frombuffer(s.recv(8), dtype=np.uint64)
    rho_mesh = s.recv(rho_len[0])
    rho_mesh = np.frombuffer(rho_mesh, dtype=np.double)
    rho_mesh.reshape(SHAPE)

    vel_len = np.frombuffer(s.recv(8), dtype=np.uint64)
    vel_mesh = s.recv(vel_len[0])
    vel_mesh = np.frombuffer(vel_mesh, dtype=np.double)

    obstacle_len = np.frombuffer(s.recv(8), dtype=np.uint64)
    obstacle_map = s.recv(obstacle_len[0])
    obstacle_map = np.frombuffer(obstacle_map, dtype=np.bool)

    print("OBSTACLES: ", obstacle_map)

    #rho_mesh = np.reshape(rho_mesh, (SHAPE[0],SHAPE[1]))
    #vel_mesh = np.reshape(vel_mesh, (2,SHAPE[0],SHAPE[1]))
    #obstacle_map = np.reshape(obstacle_map, (SHAPE[0],SHAPE[1]))


#subprocess.call(["build/bin/main.out", "config.txt"])

#rho_mesh = np.load("rho_m.npy")
#vel_mesh = np.load("vel_m.npy")
#obstacle_map = np.load("obstacle_map.npy")

#abs_vel_mesh = np.sqrt(vel_mesh[:,:,0]*vel_mesh[:,:,0] + vel_mesh[:,:,1]*vel_mesh[:,:,1])
#plt.imshow(abs_vel_mesh.transpose())
#plt.show()

#nx = rho_mesh.shape[0]
#ny = rho_mesh.shape[1]

#X, Y = np.meshgrid(np.linspace(0,nx-1,nx),np.linspace(0,ny-1,ny))

#plt.streamplot(X,Y,np.ma.array(vel_mesh[:,:,0].transpose(),mask=obstacle_map.transpose()),vel_mesh[:,:,1].transpose(), density=3)

#plt.imshow(~obstacle_map.transpose(), alpha=0.5,
#          interpolation='nearest', cmap='gray', aspect='auto')
#plt.show()

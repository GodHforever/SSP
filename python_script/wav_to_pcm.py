import wave
import numpy as np


f = wave.open('./8s_ref.wav', 'rb')

params = f.getparams()
channels, sampwidth, framerate, nframes = params[:4]
print('channles', channels, 'sampwitdh', sampwidth, 'sample rate', framerate)

f = open('./8s_ref.wav', 'rb')
f.seek(0)
f.read(44)
data = np.fromfile(f, dtype=np.int16)
data.tofile('./8s_ref.pcm')
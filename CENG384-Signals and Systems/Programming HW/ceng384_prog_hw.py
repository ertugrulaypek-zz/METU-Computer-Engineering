#!/usr/bin/env python
# -*- coding: utf-8 -*-
import numpy as np
import matplotlib
import matplotlib.pyplot as plt
from scipy.io import wavfile
import os

def tic():
    import time
    global startTime_for_tictoc
    startTime_for_tictoc = time.time()

def toc():
    import time
    if 'startTime_for_tictoc' in globals():
        print "Elapsed time is " + str(time.time() - startTime_for_tictoc) + " seconds."
    else:
        print "Toc: start time not set"

class Sound:
    def __init__(self, filename, verbose=False):
        # Load data
        sampling_rate, sound_data = wavfile.read(filename)

        #Convert data to floating values ranging from -1 to 1:
        sound_data = sound_data / (2. ** 15)

        #If two channels, then select only one channel
        if sound_data.ndim > 1:
            sound_data = sound_data[:, 0]

        self.data = sound_data
        self.sampling_rate = sampling_rate
        self.duration = sound_data.shape[0] / float(sampling_rate)
        self.number_of_samples = sound_data.shape[0]

        if verbose:
            print "Sound() is finishing with:"
            print "\t source file: ", filename
            print "\t sampling rate (Hz): ", self.sampling_rate
            print "\t duration (s): ", self.duration
            print "\t number of samples: ", self.number_of_samples


    def plot_sound(self, name=""):
        #Create an array of time values
        time_values = np.arange(0, self.number_of_samples, 1) / float(self.sampling_rate)

        #Convert to milliSeconds
        time_values = time_values * 1000

        fig, ax = plt.subplots()

        plt.plot(time_values, self.data, color='G')
        plt.xlabel('Time (ms)')
        plt.ylabel('Amplitude')
        plt.title(name)
        plt.show()


def dft(x):
    """
        Calculate Discrete Fourier Transform (DFT) for 1D signal x.

        x: A 1D signal of length N.
            Could be a Python iterable object or a Numpy array.

        Output:
            F_x: A 1D complex array, denoting the DFT of x. Its length
                should be N.
    """
    
    N = len(x)
    F_x = np.zeros(N, dtype=np.complex128)
    
    for k in range(N):
        X_k = np.zeros(1, dtype=np.complex128)[0]
        for n in range(N):
            X_k += x[n] * (np.e** ( (-2*k*n*np.pi/float(N))*(1j)) )
            
        F_x[k]= X_k

    return F_x


def calculate_spectogram(x, window_size=100, stride=50, dft_function=None):
    """
        Calculate the spectogram (S) for 1D signal x.

        x: A 1D signal of length N.
            Could be a Python iterable object or a Numpy array.
        window_size: The length of the segment for which we will calculate DFT.
        stride: How many samples will be skipped between window positions.
        dft_function: The DFT function to use. Since your implementation
            will be very slow, you should use np.fft.fft() here.

        Output:
            S: A numpy complex 2D array, denoting the S of x.
    """
    N = len(x)

    num_of_samples = int(np.floor( (N-window_size) / float(stride) ) + 1)

    s_array = np.zeros((num_of_samples,window_size), dtype=np.complex128)
    for idx in range(num_of_samples):
        s_array[idx] = dft_function(x[idx*stride: idx*stride + window_size])

    return s_array


def plot_spectogram(S, stride=50, window_size=100, duration=5, Fs=100, name=""):
    t = np.linspace(0, duration, len(S))
    f = np.arange(0, window_size / 2, dtype=np.float) * Fs / window_size # Hz

    # Since DFT is symmetric, we take only half:
    S = S[:, 0:int(window_size/2)]


    # find magnitude of S
    S = np.abs(S) #/ (window_size/2)

    # logarithmic scale (decibel)
    epsilon = 10**-13
    S = 20 * np.log10(S+epsilon) # To get rid of division-by-zero errors

    fig, ax = plt.subplots()
    plt.pcolormesh(t, f, S.transpose(), cmap=plt.get_cmap('jet'))

    #plt.yscale('symlog', linthreshy=1000, linscaley=0.25)
    #ax.yaxis.set_major_formatter(matplotlib.ticker.ScalarFormatter())

    plt.xlim(t[0], t[-1])
    plt.ylim(f[0], f[-1])

    plt.xlabel("Time (s)")
    plt.ylabel("Frequency (Hz)")

    plt.title(name)

    cbar = plt.colorbar()
    cbar.set_label("Intensity (dB)")
    plt.show()



if __name__ == "__main__":

    dirs = os.listdir('sound_samples')
    for file in dirs:

        sound_file = 'sound_samples/'+file

        signal = Sound(sound_file)

        x=signal.data[4000:5000]
        # FFT from Numpy:
        tic()
        dft_fast = np.fft.fft(x)
        toc()
        tic()
        dft_naive = dft(x)  # No need to recalculate, but let's repeat it just for clarity
        toc()
        # You should see a total difference less than 10^-10
        error = 0
        for i in range(len(dft_naive)):
            error += np.abs(dft_naive[i] - dft_fast[i])
        print "The difference (Euclidean distance) is: ", error
        print "Your are good to go!" if error < 10 ** -10 else "FAILED! Your implementation has too much error. Go back and check for errors"




        window_size = 500
        stride = 100
        x = signal.data
        Fs = signal.sampling_rate
        duration = signal.duration
        tic()
        S = calculate_spectogram(x, window_size=window_size, stride=stride, dft_function=np.fft.fft)
        toc()
        # Let's print the shape of spectogram just to make things clear
        print "S.shape: ", S.shape
        plot_spectogram(S, stride, window_size, duration=duration, Fs=Fs)


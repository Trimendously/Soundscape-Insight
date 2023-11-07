import librosa
import numpy as np
import time # For testing
import wave
import os
import struct
import sys
import subprocess
import matplotlib.pyplot as plt

try:
    requirements_file = os.path.join(os.path.dirname(__file__), "requirements.txt")
    # Uses pip subprocess to install requirements silently
    subprocess.check_call([sys.executable, "-m", "pip", "install", "-r", requirements_file], stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
except subprocess.CalledProcessError:
    print("Error installing requirements. Please check your pip installation.")

# Decorator function for measuring runtimes
def measure_runtime(func):
    def wrapper(*args, **kwargs):
        start_time = time.perf_counter()
        result = func(*args, **kwargs)
        end_time = time.perf_counter()
        elapsed_time = end_time - start_time
        elapsed_time_ms = elapsed_time * 1000  # In milliseconds
        print(f"{func.__name__} runtime (ms): {elapsed_time_ms}")
        return result
    return wrapper

# For testing if python is faster than C++ for processing
@measure_runtime
def python_process():
    wav_file = wave.open(os.path.join(os.path.dirname(__file__), '../data/audioFiles/PinkPanther60.wav'), 'rb')
    audio_data = wav_file.readframes(-1)  # Read all the audio frames
    channels = wav_file.getnchannels()
    sample_width = wav_file.getsampwidth()
    frame_rate = wav_file.getframerate()
    num_frames = wav_file.getnframes()
    compression_type = wav_file.getcomptype()
    compression_name = wav_file.getcompname()
    wav_file.close()

@measure_runtime
def main():
    # Open the binary file for reading
    with open(os.path.join(os.path.dirname(__file__), "../data/binaryFiles/audio_data.dat"), "rb") as file:
        audio_data = file.read()

    # C++ struct of aWav file
    header_format = "4s I 4s 4s I h h I I h h 4s I"

    """
    [0] char chunk_id[4];
    [1] int chunk_size;
    [2] char format[4];
    [3] char subchunk1_id[4];
    [4] int subchunk1_size;
    [5] short int audio_format;
    [6] short int num_channels;
    [7] int sample_rate;
    [8] int byte_rate;
    [9] short int block_align;
    [10] short int bits_per_sample;
    [11] char subchunk2_id[4];
    [12] int subchunk2_size;
    """

    header_size = struct.calcsize(header_format)

    with open(os.path.join(os.path.dirname(__file__), "../data/binaryFiles/header_data.dat"), "rb") as header_file:
        header_data = header_file.read(header_size)
        header_values = struct.unpack(header_format, header_data)

    audio_array = np.frombuffer(audio_data, dtype=np.int16)
    # Convert your audio data to an array of float32
    audio_array = audio_array.astype(np.float32) / 32768.0  # Assuming 16-bit signed PCM audio

    #generateSpectrogram(audio_array,header_values[7])
def generateSpectrogram(data, sample_rate):
    # Calculate the spectrogram using librosa
    spectrogram = librosa.feature.melspectrogram(y=data, sr=sample_rate)

    librosa.display.specshow(librosa.power_to_db(spectrogram, ref=np.max), y_axis='mel', x_axis='time')
    plt.colorbar(format='%+2.0f dB')
    plt.title('Mel spectrogram')
    plt.show()

main()
#python_process()


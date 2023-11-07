# Soundscape Insight (WIP)

The goal of this application is to allow users to upload various sound files(.wav) and approximate the percentage of specific noise presences within the audio clips.
## Table of Contents

- [Usage](#usage)
- [Context](#context)
- [Problems That Arose](#problems)
- [Future](#future)
- [Contributing](#contributing)
- [License](#license)


## Usage
``` bash
$ make
$ ./Audio-Insight

$ python python_scripts/main.py
```
## Context
* Uses C++ for audio processing
* Uses python for audio analysis
* No plans for hosting as the program will be too data($) intensive so it will be a downloadable executable

## Problems
- Plan is to run on large .wav files so will be data intensive
   - Using C++ for data processing to expedite process
   - Currently communicating between python and C++ files via binary files, may change in the future
- Visualizing the data in an efficient manner
   - Currently using python for visualization (librosa), but this is slower than desired

## Future
- [ ] Bundle all commands into single executable (will differ based offf OS I believe)
- [ ] Add band-pass filter (for outliers)
- [ ] Add distinct noise filtering
- [ ] Allow for spectrogram visualizations



## Contributing
Contributions are welcome.
If you use this code in your own project please just provide credit.

## License
MIT License
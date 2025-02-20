# OtoDecks

OtoDecks is a simple DJ application built using JUCE. It allows users to load, play, and manipulate audio files with features such as waveform display, volume control, speed control, and position control.

## Features

- Audio Playback: Play, pause, and stop audio tracks.
- Track Loading: Load audio tracks from the file system.
- Volume Control: Adjust the playback volume.
- Playback Speed Control: Change the speed of the audio playback.
- Waveform Display: Visual representation of the audio waveform.
- Track Scrubbing: Navigate through the track using a scrubber.
- Looping: Set loop points to repeat sections of a track.
- Crossfading: Smoothly transition between two audio tracks.
- Equalizer: Adjust the balance of frequency components.
- Playlist Management: Create and manage playlists of audio tracks.
- MIDI Control: Use MIDI devices to control playback and other features.
- Effect Processing: Apply audio effects like reverb, delay, etc.
- Recording: Record audio from input devices.
- User Interface: Intuitive and user-friendly interface for easy navigation.


## Getting Started

### Prerequisites

- JUCE library
- CMake
- A C++ compiler

### Building the Project

1. Clone the repository:
    ```sh
    git clone <repository-url>
    cd OtoDecks
    ```

2. Build the project using CMake:
    ```sh
    mkdir build
    cd build
    cmake ..
    make
    ./OtoDecks_artefacts/OtoDecks
    ```

3. Run the application:
    ```sh
    ./OtoDecks
    ```

## Usage

- Use the "LOAD" button to load an audio file.
- Use the "PLAY" and "STOP" buttons to control playback.
- Adjust the volume, speed, and position sliders to manipulate the audio.

#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <iostream>  // For standard input/output operations
#include <fstream>   // For file input/output operations
#include <chrono> // For testing
#include <filesystem> // For path construction

// WAVE PCM soundfile format
typedef struct wav_header_file
{
    char chunk_id[4]; // The file's RIFF (Resource Interchange File Format) chunk identifier.
    int chunk_size; // The size of the entire WAV file, excluding the ChunkID and ChunkSize fields themselves.
    char format[4]; // Indicates that the file is in the WAV format
    char subchunk1_id[4]; // The start of the format subchunk
    int subchunk1_size; // The size of the format subchunk
    short int audio_format;
    short int num_channels;
    int sample_rate;
    int byte_rate; // The number of bytes per second required to play the audio
    short int block_align; // The number of bytes for one audio frame
    short int bits_per_sample; // The number of bits used to represent each sample
    char subchunk2_id[4]; // The "data" identifier, indicating the start of the data subchunk
    int subchunk2_size; // The size of the data subchunk
    char *data; // Actual data
} wav_header;


// Function declarations
std::filesystem::path constructFullPath(const std::filesystem::path& relativePath);
void createNewWavFile(const std::filesystem::path& relativePath, const wav_header& header, const char* audioData, int dataSize);
void createBinaryFile(const std::filesystem::path& relativePath, const char* data, int dataSize);
void printSampleData(const wav_header& header, const char* audioData, int dataSize);


int main() 
{
    // Combines the current directory with the relative paths
    std::filesystem::path audioDataPath =  constructFullPath("data/audioFiles/PinkPanther60.wav");
    std::filesystem::path audioBinaryPath =  constructFullPath("data/binaryFiles/audio_data.dat");
    std::filesystem::path headerBinaryPath =  constructFullPath("data/binaryFiles/header_data.dat");
    
    std::ifstream file(audioDataPath, std::ios::binary);

    if (!file.is_open()) 
    {
        std::cerr << "Error opening the WAV file" << std::endl;
        return 1;
    }

    wav_header wavHeader;

    // Reads the WAV header
    file.read(reinterpret_cast<char*>(&wavHeader), sizeof(wavHeader));

    // Check if the file is in the WAV format
    if (std::string(wavHeader.chunk_id, 4) != "RIFF" || std::string(wavHeader.format, 4) != "WAVE") 
    {
        std::cerr << "Not a valid WAV file" << std::endl;
        return 1;
    }

    // Extracts metadata
    int dataSize = wavHeader.subchunk2_size;

    /////////////////////////////////////
    // Delete this in final production
    std::cout << "Number of channels: " << wavHeader.num_channels << std::endl;
    std::cout << "Sample rate: " << wavHeader.sample_rate << " Hz" << std::endl;
    std::cout << "Bits per sample: " << wavHeader.bits_per_sample << " bits" << std::endl;
    std::cout << "Data size: " << dataSize << " bytes" << std::endl;
    /////////////////////////////////////
    
    // Reads the  audio data
    char* audioData = new char[dataSize];
    file.read(audioData, dataSize);

    // Closes the file
    file.close();

    // Saves the audio data and header files as binary files
    createBinaryFile(audioBinaryPath,audioData,dataSize);   
    createBinaryFile(headerBinaryPath,reinterpret_cast<const char*>(&wavHeader),sizeof(wavHeader));

    // Cleans up allocated memory
    delete[] audioData;

    return 0;
}

// Gets the file path for the audio files
std::filesystem::path constructFullPath(const std::filesystem::path& relativePath) 
{
    std::filesystem::path currentPath = std::filesystem::current_path();
    return currentPath / relativePath;
}

// Creates a new WAV file using the header and audio data
void createNewWavFile(const std::filesystem::path& relativePath, const wav_header& header, const char* audioData, int dataSize) 
{
    std::ofstream newFile(relativePath, std::ios::binary);
    if (!newFile.is_open()) 
    {
        std::cerr << "Error creating the new WAV file" << std::endl;
        return;
    }

    // Writes the WAV header
    newFile.write(reinterpret_cast<const char*>(&header), sizeof(header));

    // Writes the audio data
    newFile.write(audioData, dataSize);

    // Closes the new file
    newFile.close();
}

// Creates a new binary file for quicker write/read times than a .wav file
void createBinaryFile(const std::filesystem::path& relativePath, const char* data, int dataSize)
{
    // Opens the binary file for writing the data
    std::ofstream binaryFile(relativePath, std::ios::binary);
    if (!binaryFile.is_open()) 
    {
        std::cerr << "Error opening the binary data file" << std::endl;
        return;
    }

    // Writes the data to the binary file
    binaryFile.write(data, dataSize);

    // Closes the binary file
    binaryFile.close();
}

void printSampleData(const wav_header& header, const char* audioData, int dataSize)
{
    // Prints the sample values
    for (int i = 0; i < dataSize; i += (header.bits_per_sample / 8)) 
    {
        // Interpret the bytes as an integer based on bits_per_sample
        short sample;
        if (header.bits_per_sample == 8) 
        {
            // For 8-bit samples (unsigned)
            sample = static_cast<short>(audioData[i]);
        } 
        else if (header.bits_per_sample == 16) 
        {
            // For 16-bit samples
            sample = static_cast<short>((audioData[i + 1] << 8) | audioData[i]);
        } 
        else 
        {
            std::cout << "More bits per sample than normal" << std::endl; // temp
        }
        std::cout << "Sample " << i / (header.bits_per_sample / 8) << ": " << sample << std::endl;
    }
}

void bandPassFilter()
{
    // Insert code here eventually :)
}
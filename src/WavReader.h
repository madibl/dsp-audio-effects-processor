#pragma once
#define DR_WAV_IMPLEMENTATION
#include "../dependencies/dr_wav.h"
#include "AudioBuffer.h"

class WavReader {
public:
    /**
     * Constructor. Initializes the input wav file.
     */
    explicit WavReader(const std::string& path) {
        if (!drwav_init_file(&wav_, path.c_str(), nullptr)) {
            throw std::runtime_error("Failed to open wav file: " + path);
        }
    }

    /** 
     * Destructor
     */
    ~WavReader() { drwav_uninit(&wav_); }

    /**
     * Getter function for wav files sample rate
     */
    uint32_t sampleRate() const { return wav_.sampleRate; }
    uint32_t channels() const { return wav_.channels; }

    AudioChunk readChunk(size_t chunk_size) {
        AudioChunk chunk;
        chunk.samples.resize(chunk_size * wav_.channels);
        drwav_uint64 framesRead = drwav_read_pcm_frames_f32(&wav_, chunk_size, chunk.samples.data());
        chunk.samples.resize(framesRead * wav_.channels);
        chunk.is_last = (framesRead < chunk_size);
        return chunk;
    }
    
private:
    drwav wav_;
};
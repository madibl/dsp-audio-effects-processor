#pragma once

#include "../dependencies/dr_wav.h"
#include "AudioBuffer.h"

class WavWriter {
public:
    /**
     * Constructor. Builds up data format and initializes the file to write to
     */
    WavWriter(const std::string& path, uint32_t sampleRate, uint32_t channels) {
        drwav_data_format format;
        format.channels = channels; // the num of individual audio streams/tracks contained in the file
        format.sampleRate = sampleRate; // how many snapshots of audio are taken a second
        format.bitsPerSample = 16; // 16 bit pcm
        format.format = DR_WAVE_FORMAT_PCM; // pulse code modulation - default format for many wav files
        format.container = drwav_container_riff; // standard wav container - 32 bit ints to describe chunk sizes
        if (!drwav_init_file_write(&wav_, path.c_str(), &format, nullptr)) {
            throw std::runtime_error("failed to open output wav file: " + path);
        }
    }

    /**
     * Destructor
     */
    ~WavWriter() { drwav_uninit(&wav_); }

    /**
     * write chunk
     */
    void writeChunk(const AudioChunk& chunk) {
        // here we convert float -> int16 before writing!
        std::vector<int16_t> pcm(chunk.samples.size());
        for (size_t i = 0; i < chunk.samples.size(); ++i) {
            // clamp to range -1.0 to 1.0 so no integer overflow
            float clamped = std::max(-1.0f, std::min(1.0f, chunk.samples[i]));
            // map float to int16 (an int 16 holds from -32768 to 32767)
            pcm[i] = static_cast<int16_t>(clamped * 32767.0f);
        }
        drwav_uint64 frames = pcm.size() / wav_.channels;
        drwav_write_pcm_frames(&wav_, frames, pcm.data());
    }

private:
    drwav wav_;
};
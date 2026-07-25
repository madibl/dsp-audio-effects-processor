### PROJECT OVERVIEW
I am building a simple C++ Multithreaded Audio DSP Pipeline! It will take a .wav file, filter it with a FIR low-pass filter and write it back out. 
The read, filter, and write stages will run on three separate threads talking through custom thread-safe queues.

## The components
# CircularBuffer
a thread safe queue. it makes sure we don't run into any buffer overflow/underflow and race conditions while we are calling our threads.

# AudioBuffer
the data unit flowing through the pipeline. 
WAV files are usually stored as 16-bit ints, but DSP math should be happening in floating point to avoid overflow and rounding artifacts. So we convert int16 to float on read and float to int16 on write!

# WavReader and WavWriter
Since i'm a newbie, just gonna be using the dr_wav library, but we shall wrap it in these reader and writer classes for further simplicity down the line.
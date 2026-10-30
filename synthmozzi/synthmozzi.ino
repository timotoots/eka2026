#include <MozziGuts.h>
#include <Oscil.h> 
#include <tables/cos2048_int8.h> // Cosine wavetable for oscillators

// Set up the Carrier and Modulator oscillators using the cosine table
Oscil<COS2048_NUM_CELLS, AUDIO_RATE> aCarrier(COS2048_DATA);
Oscil<COS2048_NUM_CELLS, AUDIO_RATE> aModulator(COS2048_DATA);

// Variable to hold the modulation intensity
long fm_intensity;

void setup() {
  // Start the Mozzi engine with default control rate
  startMozzi(); 
}

void updateControl() {
  // Read the 3 potentiometers using Mozzi's fast analog read.
  // Values will range from 0 to 1023.
  int pot1 = mozziAnalogRead(A0);
  int pot2 = mozziAnalogRead(A1);
  int pot3 = mozziAnalogRead(A2);
  
  // 1. Set Carrier Frequency (Base Pitch)
  // Map pot 1 to a frequency range of 50 Hz to 1073 Hz
  int carrier_freq = 50 + pot1; 
  aCarrier.setFreq(carrier_freq);
  
  // 2. Set Modulator Frequency (Harmonic Timbre)
  // Map pot 2 to a harmonic ratio (1 to 8) and multiply by the carrier frequency
  int ratio = (pot2 >> 7) + 1; // Bitshift divides by 128
  int mod_freq = carrier_freq * ratio; 
  aModulator.setFreq(mod_freq);
  
  // 3. Set FM Intensity (Modulation Amount)
  // Scale pot 3 up for a more dramatic FM effect
  fm_intensity = ((long)pot3) << 2; 
}

int updateAudio() {
  // Calculate the modulation amount from the modulator oscillator
  long modulation = (fm_intensity * aModulator.next()) >> 8;
  
  // Output the carrier oscillator, modulated by the calculated phase offset
  return aCarrier.phMod(modulation);
}

void loop() {
  // audioHook() is required in loop() for Mozzi to process the sound buffer
  audioHook();
}

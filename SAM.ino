#include <Arduino.h>
#include <AudioOutputI2SDAC.h>
#include "reciter.h"
#include "sam.h"
#include <i2s.h>

AudioOutputI2SDAC *out;

extern "C" {

  void OutByte(unsigned char b)
  {
    // Xvert unsigned 8 to signed 16...
    int16_t s16 = b; s16 -= 128; s16 *= 128;
    uint32_t u32 = s16 & 0xffff; u32 |= u32<<16;
    i2s_write_sample(u32);
  }

}

void setup()
{
  out = new AudioOutputI2SDAC();
  out->SetRate(22050);
  out->SetBitsPerSample(16);
  out->SetChannels(2);
  out->begin();
}

extern int main(int argc, char **argv);
void loop()
{
  static bool done = false;
  if (done) return;
  done = true;
  char input[256];
  strcpy(input, "This is a test of speech synthesis.");
  //EnableSingmode();
  //phonetic = 1;
  //SetPitch(atoi(argv[i+1]));
  //SetSpeed(atoi(argv[i+1]));
  //SetMouth(atoi(argv[i+1]));
  //SetThroat(atoi(argv[i+1]));

  for (int i=0; input[i] != 0; i++)
    input[i] = toupper((int)input[i]);

//  if (!phonetic)
//  {
    strncat(input, "[", 256);
    if (!TextToPhonemes(input)) return; // ERROR
//  } else strncat(input, "\x9b", 256);

  SetInput(input);
  SAMMain();
}


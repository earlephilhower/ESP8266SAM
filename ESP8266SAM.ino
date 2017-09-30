#include <Arduino.h>
#include <AudioOutputI2SDAC.h>
#include "reciter.h"
#include "sam.h"
#include <i2s.h>


class ESP8266SAM {

public:
  ESP8266SAM()
  {
    singmode = false;
    phonetic = false;
    pitch = 0;
    mouth = 0;
    throat = 0;
    speed = 0;
    output = NULL;
  };
  
  ~ESP8266SAM()
  {
  }
  
  void SetSingMode(bool val) { singmode = val; }
  void SetPhonetic(bool val) { phonetic = val; }
  void SetPitch(uint8_t val) { pitch = val; }
  void SetMouth(uint8_t val) { mouth = val; }
  void SetThroat(uint8_t val) { throat = val; }
  void SetSpeed(uint8_t val) { speed = val; }

  void Say(AudioOutput *out, const char *str);
  void Say_P(AudioOutput *out, const char *str) {
    char ram[256];
    strncpy_P(ram, str, 256);
    ram[255] = 0;
    Say(out, ram);
  };

private:
  static void OutputByteCallback(void *cbdata, unsigned char b);
  void OutputByte(unsigned char b);
  bool singmode;
  bool phonetic;
  int pitch;
  int speed;
  int mouth;
  int throat;
  AudioOutput *output;
};

void ESP8266SAM::OutputByteCallback(void *cbdata, unsigned char b)
{
  ESP8266SAM *sam = static_cast<ESP8266SAM*>(cbdata);
  sam->OutputByte(b);
}

void ESP8266SAM::OutputByte(unsigned char b)
{
  // Xvert unsigned 8 to signed 16...
  int16_t s16 = b; s16 -= 128; s16 *= 128;
  int16_t sample[2];
  sample[0] = s16;
  sample[1] = s16;
  while (!output->ConsumeSample(sample)) yield();
}
  
void ESP8266SAM::Say(AudioOutput *out, const char *str)
{
  if (!str || strlen(str)>255) return; // Only can speak up to 1 page worth of data...
  
  // These are fixed by the synthesis routines
  out->SetRate(22050);
  out->SetBitsPerSample(16); // Actually, it's 4-bits(!!), but we're not a SID chip...
  out->SetChannels(2); // Again, not really
  out->begin();

  // SAM settings
  EnableSingmode(singmode);
  if (speed) SetSpeed(speed);
  if (pitch) SetPitch(pitch);
  if (mouth) SetMouth(mouth);
  if (throat) SetThroat(throat);

  // Input massaging
  char input[256];
  for (int i=0; input[i] != 0; i++)
    input[i] = toupper((int)str[i]);
  input[strlen(str)] = 0;

  // To phonemes
  if (phonetic) {
    strncat(input, "\x9b", 256);
  } else {
    strncat(input, "[", 256);
    if (!TextToPhonemes(input)) return; // ERROR
  }

  // Say it!
  output = out;
  SetInput(input);
  SAMMain(OutputByteCallback, (void*)this);
}




AudioOutputI2SDAC *out;
#if 0
extern "C" {

  void OutByte(unsigned char b)
  {
    // Xvert unsigned 8 to signed 16...
    int16_t s16 = b; s16 -= 128; s16 *= 128;
    uint32_t u32 = s16 & 0xffff; u32 |= u32<<16;
    i2s_write_sample(u32);
  }

}
#endif
void setup()
{
  out = new AudioOutputI2SDAC();
  out->begin();
}

extern int main(int argc, char **argv);
void loop()
{
#if 0
  
  //static bool done = false;
  //if (done) return;
  //done = true;
  char input[256];
  static int cnt = 1;
  sprintf(input, "loop %d.", cnt++);
  //strcpy(input, "This is a test of speech synthesis.");
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
//  SAMMain();
#endif

  ESP8266SAM *sam = new ESP8266SAM;
  sam->Say(out, "This worked, holy cow!");
  delete sam;
}





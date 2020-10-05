#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef unsigned char byte;

int frames, width, height, numPixels;
byte *firePixels;

const byte NUM_COLORS = 37;
const byte flameColorGrey[NUM_COLORS] = { // J: <. 255 * (%>./)i. 37
  0, 7, 14, 21, 28, 35, 42, 49, 56, 63, 70, 77, 85, 92, 99,
  106, 113, 120, 127, 134, 141, 148, 155, 162, 170, 177,
  184, 191, 198, 205, 212, 219, 226, 233, 240, 247, 255
};
const byte flameColorRed[NUM_COLORS] = {
  0x07, 0x1F, 0x2F, 0x47, 0x57, 0x67, 0x77, 0x8F, 0x9F, 0xAF,
  0xBF, 0xC7, 0xDF, 0xDF, 0xDF, 0xD7, 0xD7, 0xD7, 0xCF, 0xCF,
  0xCF, 0xCF, 0xC7, 0xC7, 0xC7, 0xBF, 0xBF, 0xBF, 0xBF, 0xBF,
  0xB7, 0xB7, 0xB7, 0xCF, 0xDF, 0xEF, 0xFF
};
const byte flameColorGreen[NUM_COLORS] = {
  0x07, 0x07, 0x0F, 0x0F, 0x17, 0x1F, 0x1F, 0x27, 0x2F, 0x3F,
  0x47, 0x47, 0x4F, 0x57, 0x57, 0x5F, 0x5F, 0x67, 0x6F, 0x77,
  0x7F, 0x87, 0x87, 0x8F, 0x97, 0x9F, 0x9F, 0xA7, 0xA7, 0xAF,
  0xAF, 0xB7, 0xB7, 0xCF, 0xDF, 0xEF, 0xFF
};
const byte flameColorBlue[NUM_COLORS] = {
  0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07,
  0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x0F, 0x0F, 0x0F,
  0x0F, 0x17, 0x17, 0x17, 0x1F, 0x1F, 0x1F, 0x27, 0x27, 0x2F,
  0x2F, 0x2F, 0x37, 0x6F, 0x9F, 0xC7, 0xFF
};

void propagateFire() {
  int i, d, x, y, w, oldVal, newVal;

  for(i = width; i < numPixels; ++i) {
    d = rand() & 1;
    x = i % width;

    // eh
    /*
    y = i / width;
    if(y == 3 * height / 4 && ((x + (rand() & 3) / 32) % 2)) { // experimental "fire cutoff zone" to give it more texture
      d = rand() & 3;
    }
    */

    w = 0;
    switch(rand() & 7) {
      case 0:
        w = -1;
        break;
      case 1:
        w = -1;
        break;
      case 2:
        w = -1;
        break;
      case 3:
        w = -1;
        break;
      case 4:
        w = 0;
        break;
      case 5:
        w = 0;
        break;
      case 6:
        w = 0;
        break;
      case 7:
        w = 1;
        break;
    }
    oldVal = firePixels[i];
    newVal = oldVal < d ? 0 : oldVal - d;
    firePixels[i - width + w] =  newVal;
  }
}

void writeFirePixelsGrey(char *fileName) {
  int x, y, i;
  FILE *file = fopen(fileName, "w");

  // pgm header
  fprintf(file, "P2\n# %s\n%d %d\n255\n", fileName, width, height);

  // print pixel values
  i = 0;
  for(y = 0; y < height; ++y) {
    for(x = 0; x < width; ++x) {
      fprintf(file, "%-4d", flameColorGrey[firePixels[i++]]);
    }
    fprintf(file, "\n");
  }
  fclose(file);
}

void writeFirePixelsRgb(char *fileName) {
  int x, y, i;
  FILE *file = fopen(fileName, "w");

  // pgm header
  fprintf(file, "P3\n# %s\n%d %d\n255\n", fileName, width, height);

  // print pixel values
  i = 0;
  for(y = 0; y < height; ++y) {
    for(x = 0; x < width; ++x) {
      fprintf(file, "%-4d", flameColorRed[firePixels[i]]);
      fprintf(file, "%-4d", flameColorGreen[firePixels[i]]);
      fprintf(file, "%-4d", flameColorBlue[firePixels[i]]);
      fprintf(file, " ");
      i++;
    }
    fprintf(file, "\n");
  }
  fclose(file);
}

int main(int argc, char **argv) {
  const int DEFAULT_W = 20;
  const int DEFAULT_H = 20;

  int i, t;

  char fileName[64];
  char frameNameFormat[64];

  if(argc == 1) {
    printf("Usage: %s <frames> [<width> <height>]\n", argv[0]);
    return 0;
  } 
  if(argc > 1) {
    frames = atoi(argv[1]);
    width  = DEFAULT_W;
    height = DEFAULT_H;
  }
  if(argc > 2) {
    width  = atoi(argv[2]);
    height = atoi(argv[2]);
  }
  if(argc > 3) {
    height = atoi(argv[3]);
  }

  if(height <= 0 || width <= 0) {
    printf("Invalid dimensions (%d x %d)!\n", width, height);
    return 1;
  }

  sprintf(frameNameFormat, "frame_%%0%dd.pgm", (int)(log10((double)frames) + 1));

  numPixels = width * height;

  // init firePixels
  firePixels = (byte *)malloc(numPixels * sizeof(byte));
  for(i = 0; i < numPixels - width; ++i) {
    firePixels[i] = 0;
  }
  for(i = numPixels - width; i < numPixels; ++i) {
    firePixels[i] = NUM_COLORS - 1;
  }

  // calculate and write frames
  for(t = 0; t < frames; ++t) {
    propagateFire();

    sprintf(fileName, frameNameFormat, t);
    printf("%s\n", fileName);
    // writeFirePixelsRgb(fileName);
    writeFirePixelsRgb(fileName);
  }


  // cleanup
  free(firePixels);
  return 0;
}

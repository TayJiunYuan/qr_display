#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <SPI.h>

// Pin definitions
#define TFT_CS 5
#define TFT_RST 26
#define TFT_DC 25

#define BUTTON_PIN 4

#define SMALL_QR_PIXEL_SIZE 3
#define SMALL_QR_OFFSET_HEIGHT 5
#define SMALL_QR_OFFSET_LENGTH 44

#define SIZE_4_BIG_QR_PIXEL_SIZE 4
#define SIZE_4_BIG_QR_OFFSET_HEIGHT 60
#define SIZE_4_BIG_QR_OFFSET_LENGTH 14

#define SIZE_3_BIG_QR_PIXEL_SIZE 3
#define SIZE_3_BIG_QR_OFFSET_HEIGHT 80
#define SIZE_3_BIG_QR_OFFSET_LENGTH 26

int currentQRIndex = 0;
const int totalQRs = 4;
bool lastButtonState = HIGH;

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

const uint8_t small_qr_1[13][13] = {
    {1, 0, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1},
    {0, 1, 0, 1, 1, 0, 1, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 1, 1, 0, 1, 0, 1, 1, 1, 0, 1},
    {1, 1, 1, 1, 0, 0, 1, 0, 1, 1, 1, 0, 1},
    {0, 1, 1, 0, 0, 0, 1, 0, 1, 1, 1, 0, 1},
    {0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1},
    {1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0},
    {1, 0, 0, 1, 1, 0, 0, 1, 0, 0, 1, 1, 1},
    {1, 0, 0, 1, 1, 1, 1, 0, 1, 1, 0, 0, 0},
    {0, 1, 1, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1},
    {0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 1, 1, 0},
    {1, 0, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1},
};

const uint8_t big_qr_1[25][25] = {
    {1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 0, 1, 0, 1},
    {1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1},
    {1, 0, 1, 1, 1, 0, 1, 0, 0, 1, 0, 1, 1, 0, 1, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1},
    {1, 0, 1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 1, 1, 0, 1, 1, 0, 0, 1, 0, 0, 1, 0, 1},
    {1, 0, 1, 1, 1, 0, 1, 0, 1, 0, 0, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1},
    {1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 1, 0, 1, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0},
    {1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 0, 0, 1, 0, 0, 1, 0, 1, 0, 1, 1, 0, 1, 1},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 1, 0, 0, 0, 1, 0, 1, 1, 1},
    {0, 0, 1, 1, 0, 1, 1, 1, 1, 1, 0, 0, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1},
    {0, 1, 0, 1, 0, 1, 0, 1, 1, 0, 1, 0, 0, 1, 0, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1},
    {1, 1, 0, 1, 1, 0, 1, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1},
    {1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0, 1, 1, 1},
    {0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 0, 0, 0, 1, 0, 0},
    {0, 0, 0, 1, 0, 1, 0, 0, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 0, 0, 0, 1, 0},
    {1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 0, 0, 1, 1, 1},
    {0, 0, 0, 0, 1, 0, 0, 1, 0, 1, 1, 0, 0, 0, 1, 0, 0, 1, 0, 1, 0, 1, 0, 1, 1},
    {0, 1, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0, 1, 0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 1},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0},
    {1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1},
    {1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 1, 1, 0, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 1},
    {1, 0, 1, 1, 1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 0, 0, 0, 0, 1, 0, 1, 1, 1, 0, 1},
    {1, 0, 1, 1, 1, 0, 1, 0, 0, 0, 0, 1, 0, 0, 1, 1, 1, 0, 1, 0, 1, 1, 1, 0, 1},
    {1, 0, 1, 1, 1, 0, 1, 0, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0, 1, 1, 1, 0, 1},
    {1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 1, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 1},
    {1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 0, 0, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1},
};

const uint8_t big_qr_2[25][25] = {
    {1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 0, 0, 1, 0, 1, 1, 0, 1, 0, 1, 0, 0, 1, 0, 1},
    {1, 0, 0, 0, 0, 0, 1, 0, 1, 1, 0, 1, 0, 0, 1, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1},
    {1, 0, 1, 1, 1, 0, 1, 0, 0, 1, 0, 1, 1, 1, 0, 1, 0, 1, 0, 0, 1, 0, 0, 1, 0},
    {1, 0, 1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 0, 1, 1, 0, 0},
    {1, 0, 1, 1, 1, 0, 1, 0, 1, 0, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0},
    {1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 1, 1, 0, 1, 0, 0, 0, 1, 1, 0, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 1, 0, 1, 0, 0, 0, 1, 1, 0, 0, 0},
    {0, 0, 1, 1, 0, 1, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0},
    {0, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 0, 0},
    {1, 1, 0, 1, 1, 0, 1, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1},
    {1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0, 1, 1, 1},
    {0, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 0, 1, 0, 0, 1, 0, 1, 0, 0, 0, 0, 1, 0, 0},
    {0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1, 0, 0, 0, 1, 0},
    {1, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 0, 1, 0, 1, 1, 0, 0, 0, 1, 0, 0, 0},
    {1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 0, 0, 1, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1},
    {0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 1, 1, 1, 0, 1, 1, 0, 1, 0, 0, 1, 0, 1, 0, 1},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0},
    {1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1},
    {1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 1},
    {1, 0, 1, 1, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 1, 0, 1},
    {1, 0, 1, 1, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 1, 1, 0, 1},
    {1, 0, 1, 1, 1, 0, 1, 0, 1, 0, 0, 1, 0, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 0, 1},
    {1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 1},
    {1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1},
};

void drawQRCodes(int pixelSizeBig, int offsetXBig, int offsetYBig, const uint8_t bigQR[25][25])
{
  for (int y = 0; y < 13; y++)
  {
    for (int x = 0; x < 13; x++)
    {
      if (small_qr_1[y][x])
      {
        tft.fillRect(SMALL_QR_OFFSET_LENGTH + x * SMALL_QR_PIXEL_SIZE, SMALL_QR_OFFSET_HEIGHT + y * SMALL_QR_PIXEL_SIZE, SMALL_QR_PIXEL_SIZE, SMALL_QR_PIXEL_SIZE, ST77XX_BLACK);
      }
      else
      {
        tft.fillRect(SMALL_QR_OFFSET_LENGTH + x * SMALL_QR_PIXEL_SIZE, SMALL_QR_OFFSET_HEIGHT + y * SMALL_QR_PIXEL_SIZE, SMALL_QR_PIXEL_SIZE, SMALL_QR_PIXEL_SIZE, ST77XX_WHITE);
      }
    }
  };

  for (int y = 0; y < 25; y++)
  {
    for (int x = 0; x < 25; x++)
    {
      if (bigQR[y][x])
      {
        tft.fillRect(offsetXBig + x * pixelSizeBig, offsetYBig + y * pixelSizeBig, pixelSizeBig, pixelSizeBig, ST77XX_BLACK);
      }
      else
      {
        tft.fillRect(offsetXBig + x * pixelSizeBig, offsetYBig + y * pixelSizeBig, pixelSizeBig, pixelSizeBig, ST77XX_WHITE);
      }
    }
  }
}

void qrCodeLoop(int index)
{
  tft.fillScreen(ST77XX_WHITE);
  if (index == 0)
  {
    drawQRCodes(SIZE_3_BIG_QR_PIXEL_SIZE, SIZE_3_BIG_QR_OFFSET_LENGTH, SIZE_3_BIG_QR_OFFSET_HEIGHT, big_qr_1);
  }
  else if (index == 1)
  {
    drawQRCodes(SIZE_4_BIG_QR_PIXEL_SIZE, SIZE_4_BIG_QR_OFFSET_LENGTH, SIZE_4_BIG_QR_OFFSET_HEIGHT, big_qr_1);
  }
  else if (index == 2)
  {
    drawQRCodes(SIZE_3_BIG_QR_PIXEL_SIZE, SIZE_3_BIG_QR_OFFSET_LENGTH, SIZE_3_BIG_QR_OFFSET_HEIGHT, big_qr_2);
  }
  else
  {
    drawQRCodes(SIZE_4_BIG_QR_PIXEL_SIZE, SIZE_4_BIG_QR_OFFSET_LENGTH, SIZE_4_BIG_QR_OFFSET_HEIGHT, big_qr_2);
  }
}

void setup()
{
  Serial.begin(9600);
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  tft.initR(INITR_BLACKTAB); // ST7735S usually uses this tab
  tft.setRotation(0);        // Landscape
  tft.fillScreen(ST77XX_WHITE);
}

void loop()
{
  bool currentButtonState = digitalRead(BUTTON_PIN);
  Serial.print(currentButtonState);

  if (lastButtonState == HIGH && currentButtonState == LOW) // detect button press on falling edge
  {
    currentQRIndex = (currentQRIndex + 1) % totalQRs;
    qrCodeLoop(currentQRIndex);
    delay(200);
  }

  lastButtonState = currentButtonState;
}

// codigo para nave espacial modular
/* NAVE:   _
          /_\  PUNTA
          |_|  MODULO 1
          |_|  MODULO 2
          |O|  CAPSULA
          |_|  MODULO 3
          |_|  MODULO 4
         |___| PROPULSOR
*/

// BIBLIOTECAS INCLUIDAS
#include <SD.h>
#include <SPI.h>
#include <FS.h>
#include <SPIFFS.h>
#include <TFT_eSPI.h>
#include <AnimatedGIF.h>

// --PINES DE MODULOS--
// MODULO 1 (UP LVL2)
int pinesMod1[3] = { 13, 12, 14 };
// MODULO 2 (UP LVL1)
int pinesMod2[3] = { 27, 26, 25 };
// MODULO 3 (DOWN LVL1)
int pinesMod3[3] = { 34, 21, 22 };
// MODULO 4 (DOWN LVL2)
int pinesMod4[3] = { 33, 32, 35 };

// PANTALLA
TFT_eSPI tft = TFT_eSPI();
// LECTOR SD
#define SD_CS_PIN 5

//gifs
AnimatedGIF gif;
File gifFile;
const char *filenameIntro = "/logo.GIF";
const char *filenameLogoOUT = "/logoOUT.GIF";
const char *filenameWait = "/prueba.GIF";
const char *filenameReady = "/prueba2.GIF";


void setup() {

  // iniciar SD
  if (!SD.begin()) {
    Serial.println("Card Mount Failed");
    return;
  }

  // configurar pines modulos
  pinMode(pinesMod1[0], INPUT);
  pinMode(pinesMod1[1], INPUT);
  pinMode(pinesMod1[2], INPUT);

  pinMode(pinesMod2[0], INPUT);
  pinMode(pinesMod2[1], INPUT);
  pinMode(pinesMod2[2], INPUT);

  pinMode(pinesMod3[0], INPUT);
  pinMode(pinesMod3[1], INPUT);
  pinMode(pinesMod3[2], INPUT);

  pinMode(pinesMod4[0], INPUT);
  pinMode(pinesMod4[1], INPUT);
  pinMode(pinesMod4[3], INPUT);

  // inicio serial
  Serial.begin(115200);

  //inicio pantalla
  tft.begin();
  tft.setRotation(2);  // Adjust Rotation of your screen (0-3)
  tft.fillScreen(TFT_BLACK);

  // Initialize SPIFFS
  Serial.println("Initialize SPIFFS...");
  if (!SPIFFS.begin(true)) {
    Serial.println("SPIFFS initialization failed!");
  }

  Serial.println("Formatting SPIFFS...");
  SPIFFS.format();  // This will erase all the files, change as needed, SPIFFs is non-volatile memory
  Serial.println("SPIFFS formatted successfully.");

  // Open GIF file from SD card
  Serial.println("Openning GIF file from SD card...");

  ///////////// DEBO TRANSFORMAR ESTO EN UNA FUNCION
  // GIF INTRO
  File sdFileIntro = SD.open(filenameIntro);
  File spiffsFileIntro = SPIFFS.open(filenameIntro, FILE_WRITE, true);
  Serial.println("Copy GIF in SPIFFS...");
  byte bufferA[512];
  while (sdFileIntro.available()) {
    int bytesReadA = sdFileIntro.read(bufferA, sizeof(bufferA));
    spiffsFileIntro.write(bufferA, bytesReadA);
  }
  spiffsFileIntro.close();
  sdFileIntro.close();

  // GIF LOGO OUT
  File sdFileLogoOUT = SD.open(filenameLogoOUT);
  File spiffsFileLogoOUT = SPIFFS.open(filenameLogoOUT, FILE_WRITE, true);
  Serial.println("Copy GIF in SPIFFS...");
  byte bufferlogoOUT[512];
  while (sdFileLogoOUT.available()) {
    int bytesReadLogoOUT = sdFileLogoOUT.read(bufferlogoOUT, sizeof(bufferlogoOUT));
    spiffsFileLogoOUT.write(bufferlogoOUT, bytesReadLogoOUT);
  }
  spiffsFileLogoOUT.close();
  sdFileLogoOUT.close();

  // GIF WAIT
  File sdFileWait = SD.open(filenameWait);
  File spiffsFileWait = SPIFFS.open(filenameWait, FILE_WRITE, true);
  Serial.println("Copy GIF in SPIFFS...");
  byte bufferB[512];
  while (sdFileWait.available()) {
    int bytesReadB = sdFileWait.read(bufferB, sizeof(bufferB));
    spiffsFileWait.write(bufferB, bytesReadB);
  }
  spiffsFileWait.close();
  sdFileWait.close();

  // GIF READY
  File sdFileReady = SD.open(filenameReady);
  File spiffsFileReady = SPIFFS.open(filenameReady, FILE_WRITE, true);
  Serial.println("Copy GIF in SPIFFS...");
  byte bufferC[512];
  while (sdFileReady.available()) {
    int bytesReadC = sdFileReady.read(bufferC, sizeof(bufferC));
    spiffsFileReady.write(bufferC, bytesReadC);
  }
  spiffsFileReady.close();
  sdFileReady.close();

  // Initialize the GIF
  Serial.println(millis());
  gif.begin(BIG_ENDIAN_PIXELS);
}

int sumaModulosAntes = -1;

void loop() {

  // mostrar logo por 2 segundos
  while (millis() < 20000) {
    if (gif.open(filenameIntro, fileOpen, fileClose, fileRead, fileSeek, GIFDraw)) {
      tft.startWrite();
      while (gif.playFrame(true, NULL)) {
      }
      gif.close();
      tft.endWrite();
    }
  }
  // mostrar animacion de encendido de la nave
  if (gif.open(filenameLogoOUT, fileOpen, fileClose, fileRead, fileSeek, GIFDraw)) {
    tft.startWrite();
    while (gif.playFrame(true, NULL)) {
    }
    gif.close();
    tft.endWrite();
  }
  unsigned long timeInit = millis();
  while (true) {
    unsigned long timeNow = millis() - timeInit;

    if (cualModulo(1) + cualModulo(2) + cualModulo(3) + cualModulo(4) == 0 && sumaModulosAntes != 0) {
      // animacion de espera
      tft.fillScreen(TFT_BLACK);
      tft.setCursor(10, 100, 2);
      tft.setTextColor(TFT_WHITE);
      tft.setTextSize(2);
      tft.println("ensambla la nave");
      tft.setCursor(32, 130, 2);
      tft.println("para tu mision");
      /*if (gif.open(filenameWait, fileOpen, fileClose, fileRead, fileSeek, GIFDraw)) {
        tft.startWrite();
        while (gif.playFrame(true, NULL)) {
        }
        gif.close();
        tft.endWrite();
      }*/
    } else {
      // muestra el codigo de configuracion de la nave porla pantalla
      // luego animacion de nave lista
      if (cualModulo(1) + cualModulo(2) + cualModulo(3) + cualModulo(4) != sumaModulosAntes) {

        tft.fillScreen(TFT_BLACK);
        tft.setCursor(30, 110, 2);
        tft.setTextColor(TFT_WHITE);
        tft.setTextSize(2);

        /*
      // codigo de prueba
      if (cualModulo(2) != 0){
        String texto;
        switch (cualModulo(2)) {
        case 1:
        texto = "Gravedad";
        break;
        case 2:
        texto = "Energia";
        break;
        case 3:
        texto = "Calor";
        break;
        case 4:
        texto = "Frio";
        break;
        case 5:
        break;
        case 6:
        break;
        case 7:
        texto = "Sonda";
        break;
        }
        tft.println(texto);
      }*/

        int moduloActivo[4];
        int modulosEscogidos[4];

        for (int i = 0; i < 4; i++) {
          int numeroMod = cualModulo(i + 1);
          if (numeroMod == 7) {
            modulosEscogidos[i] = 4;
          } else {
            modulosEscogidos[i] = numeroMod - 1;
          }
          if (numeroMod == 0) {
            moduloActivo[i] = 0;
            modulosEscogidos[i] = random(0, 9);
          } else {
            moduloActivo[i] = random(1, 9);
          }
        }

        tft.println(String(random(0, 9)) + String(moduloActivo[0]) + String(modulosEscogidos[0]) + String(moduloActivo[1]) + String(modulosEscogidos[1]) + String(random(0, 9)) + String(moduloActivo[2]) + String(modulosEscogidos[2]) + String(moduloActivo[3]) + String(modulosEscogidos[3]) + String(random(0, 9)));
        
      }

      /*if (gif.open(filenameReady, fileOpen, fileClose, fileRead, fileSeek, GIFDraw)) {
        tft.startWrite();
        while (gif.playFrame(true, NULL)) {
        }
        gif.close();
        tft.endWrite();
      }*/
    }
    sumaModulosAntes = cualModulo(1) + cualModulo(2) + cualModulo(3) + cualModulo(4);
    Serial.println(sumaModulosAntes);
  }
}


// funcion para identificar el modulo conectado
// transformando de binario (3 bits) a un numero entre 0 y 7
int cualModulo(int mod) {
  int n = 0;
  if (mod == 1) {
    if (digitalRead(pinesMod1[0])) n += 4;
    if (digitalRead(pinesMod1[1])) n += 2;
    if (digitalRead(pinesMod1[2])) n += 1;
  }
  if (mod == 2) {
    if (digitalRead(pinesMod2[0])) n += 4;
    if (digitalRead(pinesMod2[1])) n += 2;
    if (digitalRead(pinesMod2[2])) n += 1;
  }
  if (mod == 3) {
    if (digitalRead(pinesMod3[0])) n += 4;
    if (digitalRead(pinesMod3[1])) n += 2;
    if (digitalRead(pinesMod3[2])) n += 1;
  }
  if (mod == 4) {
    if (digitalRead(pinesMod4[0])) n += 4;
    if (digitalRead(pinesMod4[1])) n += 2;
    if (digitalRead(pinesMod4[2])) n += 1;
  }
  return n;
}

///////// FUNCIONES DE LOS GIF //////////

void *fileOpen(const char *filename, int32_t *pFileSize) {
  gifFile = SPIFFS.open(filename, FILE_READ);
  *pFileSize = gifFile.size();
  if (!gifFile) {
    Serial.println("Failed to open GIF file from SPIFFS!");
  }
  return &gifFile;
}

void fileClose(void *pHandle) {
  gifFile.close();
}

int32_t fileRead(GIFFILE *pFile, uint8_t *pBuf, int32_t iLen) {
  int32_t iBytesRead;
  iBytesRead = iLen;
  if ((pFile->iSize - pFile->iPos) < iLen)
    iBytesRead = pFile->iSize - pFile->iPos;
  if (iBytesRead <= 0)
    return 0;

  gifFile.seek(pFile->iPos);
  int32_t bytesRead = gifFile.read(pBuf, iLen);
  pFile->iPos += iBytesRead;

  return bytesRead;
}

int32_t fileSeek(GIFFILE *pFile, int32_t iPosition) {
  if (iPosition < 0)
    iPosition = 0;
  else if (iPosition >= pFile->iSize)
    iPosition = pFile->iSize - 1;
  pFile->iPos = iPosition;
  gifFile.seek(pFile->iPos);
  return iPosition;
}

/////////////////////////////////////////

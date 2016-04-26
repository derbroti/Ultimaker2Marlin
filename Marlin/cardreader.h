#ifndef CARDREADER_H
#define CARDREADER_H

#define MAX_DIR_DEPTH 10
#define IS_SD_INSERTED (READ(SDCARDDETECT)==0)
#define IS_SD_PRINTING (card.sdprinting)

#include "Sd2CardExt.h"
#include "SdFileExt.h"

enum LsAction {LS_SerialPrint,LS_Count,LS_GetFilename};
class CardReader
{
public:
  CardReader();

  void initsd();
  void write_command(char *buf);
  bool write_string(char* buffer);
  //files auto[0-9].g on the sd card are performed in a row
  //this is to delay autostart and hence the initialisaiton of the sd card to some seconds after the normal init, so the device is available quick after a reset

  void checkautostart(bool x);
  void openFile(const char* name,bool read);
  void openLogFile(const char* name);
  void removeFile(const char* name);
  void closefile();
  void release();
  void startFileprint();
  void pauseSDPrint();
  void getStatus();
  void printingHasFinished();

  void getfilename(const uint8_t nr);
  uint16_t getnrfilenames();


  void ls();
  void chdir(const char * relpath);
  void updir();
  void setroot();

//   FORCE_INLINE uint8_t reset() { return card.reset(); }
  FORCE_INLINE bool isFileOpen() { return file.isOpen(); }
  FORCE_INLINE bool eof() { return sdpos>=filesize ;}
  FORCE_INLINE int16_t get() {  sdpos = file.curPosition();return (int16_t)file.read();}
  FORCE_INLINE int16_t fgets(char* str, int16_t num) { return file.fgets(str, num, NULL); }
  FORCE_INLINE void setIndex(long index) {sdpos = index;file.seekSet(index);}
  FORCE_INLINE uint8_t percentDone(){if(!isFileOpen()) return 0; if(filesize) return sdpos/((filesize+99)/100); else return 0;}
  FORCE_INLINE char* getWorkDirName(){workDir.getFilename(filename);return filename;}
  FORCE_INLINE bool atRoot() { return workDirDepth==0; }
  FORCE_INLINE uint32_t getFilePos() { return sdpos; }
  FORCE_INLINE uint32_t getFileSize() { return filesize; }
  FORCE_INLINE bool isOk() { return cardOK && card.errorCode() == 0; }
  FORCE_INLINE int errorCode() { return card.errorCode(); }
  FORCE_INLINE void clearError() { card.clearError(); }
  FORCE_INLINE void updateSDInserted()
  {
    bool newInserted = IS_SD_INSERTED;
    if (sdInserted != newInserted)
    {
      if (insertChangeDelay)
        insertChangeDelay--;
      else
        sdInserted = newInserted;
    }else{
      insertChangeDelay = 1000 / 25;
    }
  }

public:
  bool saving;
  bool logging;
  bool sdprinting;
  bool pause;
  bool sdInserted;
  char filename[13];
  char longFilename[LONG_FILENAME_LENGTH];
  bool filenameIsDir;
  int lastnr; //last number of the autostart;
private:
  bool cardOK;
  SdFileExt root,*curDir,workDir,workDirParents[MAX_DIR_DEPTH];
  uint8_t workDirDepth;
  uint8_t insertChangeDelay;
  Sd2CardExt card;
  SdVolume volume;
  SdFileExt file;
  uint32_t filesize;
  //int16_t n;
  unsigned long autostart_atmillis;
  uint32_t sdpos ;

  bool autostart_stilltocheck; //the sd start is delayed, because otherwise the serial cannot answer fast enought to make contact with the hostsoftware.

  LsAction lsAction; //stored for recursion.
  int16_t nrFiles; //counter for the files in the current directory and recycled as position counter for getting the nrFiles'th name in the directory.
  char* diveDirName;
  void lsDive(SdFileExt &parent, SdFileExt** parents, uint8_t dirDepth);
};
extern CardReader card;

#endif

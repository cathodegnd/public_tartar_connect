
#ifndef _LIB_RS485_H_
#define _LIB_RS485_H_

#include <Arduino.h>
#include <stdint.h>
#include <HardwareSerial.h>
#include "lib_param.h"

#ifdef ARDUINO_ARCH_SAM
#include "lib_dmaserial.h"
#endif

typedef enum{
  LIB_OK,
  LIB_ERROR
}LibReturnStateEnum;

typedef enum{
  LIB_FALSE,
  LIB_TRUE
}LibBoolEnum;

#ifndef TIME_CLASS_H
#define TIME_CLASS_H
class Time{
  public:
  void resetTimeout(const uint32_t _timeout);
  bool isTimeOut(void);
  private:
  uint32_t timeout = 0;
  uint32_t count = 0;
};
#endif

class Rs485{
  public:
  Rs485(HardwareSerial *_pSerial);
  void initRs485(void);
  LibReturnStateEnum setSendData(int *_pData,const int _data_len);
  LibReturnStateEnum setReceiveData(int *_pData,const int _data_len);
  LibReturnStateEnum setSendDataWithCobsAndChecksum(const int *_pData,const int _data_len);
  LibReturnStateEnum sendData(const uint32_t _timeout);
  LibReturnStateEnum serialWrite(const int _data);
  int serialRead(const int _timeout);
  LibReturnStateEnum receiveDataWithCobsAndChecksum(const uint32_t _timeout);
  LibReturnStateEnum getReceiveData(int *_receive_len,int *_pData,const int _data_len);
  LibReturnStateEnum getSerialUntilZero(int *_receive_len,int *_pTarget,const int _target_len,const uint32_t _timeout);
  int serialReadAvailable(void);

  private:
  LibReturnStateEnum decodeData(int *pDecode_length,int *pTarget,const int target_length,int *pSource,const int source_length);
  int getSendSize(void);
  LibReturnStateEnum setSendSize(const int _size);
  int getReceiveSize(void);
  LibReturnStateEnum setReceiveSize(const int _size);
//  int serialWriteAvailable(void);
  LibBoolEnum isByte(int *_pData,const int _data_len);
  void clearSendData(void);
  void clearReceiveData(void);
  void clearLen(int *_pData,const int _data_len);
  LibReturnStateEnum decodeCobs(int *_pConvert_len,int *_pTarget,const int _target_len,int *_pSource,const int _source_len);
  LibReturnStateEnum encodeCobs(int *_pConvert_len,int *_pTarget,const int _target_len,int *_pSource,const int _source_len);
  int getCrc8(const int *s, const int start_pos, const int end_pos);

#ifdef ARDUINO_ARCH_SAM
  DmaSerial *ds;
#else
  HardwareSerial *hs;
#endif
  int pSend_data[SEND_DATA_LEN] = {0};
  int pReceive_data[RECEIVE_DATA_LEN] = {0};
  int send_size = 0;
  int receive_size = 0;
};

#endif

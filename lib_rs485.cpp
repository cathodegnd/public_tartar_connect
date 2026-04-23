#include "lib_rs485.h"

//#define DEBUG_RS485
//#define ARDUINO_ARCH_SAM
#ifdef ARDUINO_ARCH_SAM
DmaSerial dma_serial1 = DmaSerial((Uart*) USART0, ID_USART0); //uart1
DmaSerial dma_serial2 = DmaSerial((Uart*) USART1, ID_USART1); //uart2
DmaSerial dma_serial3 = DmaSerial((Uart*) USART3, ID_USART3); //uart3
#endif

void Time::resetTimeout(const uint32_t _timeout) {
	timeout = _timeout;
	count = millis();
}
bool Time::isTimeOut(void) {
	if (millis() - count > timeout) {
		return true;
	}
	return false;
}

int crc8_table[256] = { 0x00, 0x85, 0x8F, 0x0A, 0x9B, 0x1E, 0x14, 0x91, 0xB3,
		0x36, 0x3C, 0xB9, 0x28, 0xAD, 0xA7, 0x22, 0xE3, 0x66, 0x6C, 0xE9, 0x78,
		0xFD, 0xF7, 0x72, 0x50, 0xD5, 0xDF, 0x5A, 0xCB, 0x4E, 0x44, 0xC1, 0x43,
		0xC6, 0xCC, 0x49, 0xD8, 0x5D, 0x57, 0xD2, 0xF0, 0x75, 0x7F, 0xFA, 0x6B,
		0xEE, 0xE4, 0x61, 0xA0, 0x25, 0x2F, 0xAA, 0x3B, 0xBE, 0xB4, 0x31, 0x13,
		0x96, 0x9C, 0x19, 0x88, 0x0D, 0x07, 0x82,

		0x86, 0x03, 0x09, 0x8C, 0x1D, 0x98, 0x92, 0x17, 0x35, 0xB0, 0xBA, 0x3F,
		0xAE, 0x2B, 0x21, 0xA4, 0x65, 0xE0, 0xEA, 0x6F, 0xFE, 0x7B, 0x71, 0xF4,
		0xD6, 0x53, 0x59, 0xDC, 0x4D, 0xC8, 0xC2, 0x47, 0xC5, 0x40, 0x4A, 0xCF,
		0x5E, 0xDB, 0xD1, 0x54, 0x76, 0xF3, 0xF9, 0x7C, 0xED, 0x68, 0x62, 0xE7,
		0x26, 0xA3, 0xA9, 0x2C, 0xBD, 0x38, 0x32, 0xB7, 0x95, 0x10, 0x1A, 0x9F,
		0x0E, 0x8B, 0x81, 0x04,

		0x89, 0x0C, 0x06, 0x83, 0x12, 0x97, 0x9D, 0x18, 0x3A, 0xBF, 0xB5, 0x30,
		0xA1, 0x24, 0x2E, 0xAB, 0x6A, 0xEF, 0xE5, 0x60, 0xF1, 0x74, 0x7E, 0xFB,
		0xD9, 0x5C, 0x56, 0xD3, 0x42, 0xC7, 0xCD, 0x48, 0xCA, 0x4F, 0x45, 0xC0,
		0x51, 0xD4, 0xDE, 0x5B, 0x79, 0xFC, 0xF6, 0x73, 0xE2, 0x67, 0x6D, 0xE8,
		0x29, 0xAC, 0xA6, 0x23, 0xB2, 0x37, 0x3D, 0xB8, 0x9A, 0x1F, 0x15, 0x90,
		0x01, 0x84, 0x8E, 0x0B,

		0x0F, 0x8A, 0x80, 0x05, 0x94, 0x11, 0x1B, 0x9E, 0xBC, 0x39, 0x33, 0xB6,
		0x27, 0xA2, 0xA8, 0x2D, 0xEC, 0x69, 0x63, 0xE6, 0x77, 0xF2, 0xF8, 0x7D,
		0x5F, 0xDA, 0xD0, 0x55, 0xC4, 0x41, 0x4B, 0xCE, 0x4C, 0xC9, 0xC3, 0x46,
		0xD7, 0x52, 0x58, 0xDD, 0xFF, 0x7A, 0x70, 0xF5, 0x64, 0xE1, 0xEB, 0x6E,
		0xAF, 0x2A, 0x20, 0xA5, 0x34, 0xB1, 0xBB, 0x3E, 0x1C, 0x99, 0x93, 0x16,
		0x87, 0x02, 0x08, 0x8D, };

////////////public////////////TODO:受信処理書いてない...
Rs485::Rs485(HardwareSerial *_pSerial) {
#ifdef ARDUINO_ARCH_SAM
	if (_pSerial == &Serial3) {
		ds = &dma_serial3;
	} else if (_pSerial == &Serial2) {
		ds = &dma_serial2;
	} else if (_pSerial == &Serial1) {
		ds = &dma_serial1;
	}
	/*
	 switch((int)_pSerial){
	 default:
	 case (int)(&Serial3):
	 ds = &dma_serial3;
	 break;
	 case (int)(&Serial2):
	 ds = &dma_serial2;
	 break;
	 case (int)(&Serial1):
	 ds = &dma_serial1;
	 break;
	 }*/
#else
  hs = _pSerial;
#endif
}

void Rs485::initRs485(void) {
#ifdef DEBUG_RS485
	Serial.begin(115200);//////////////////////////////////////////////
#endif
#ifdef ARDUINO_ARCH_SAM
	ds->begin(115200);
#else
	hs->begin(115200);
#endif
}
LibReturnStateEnum Rs485::setSendData(int *_pData, const int _data_len) { //lenはデータの個数を表す
	clearSendData();
	if (isByte(_pData, _data_len) == LIB_FALSE
			|| setSendSize(_data_len) == LIB_ERROR) {
		return LIB_ERROR;
	}
	for (int i = 0; i < _data_len; i++) {
		pSend_data[i] = _pData[i];
	}
	return LIB_OK;
}

LibReturnStateEnum Rs485::setReceiveData(int *_pData, const int _data_len) { //lenはデータの個数を表す
	clearReceiveData();
	if (isByte(_pData, _data_len) == LIB_FALSE
			|| setReceiveSize(_data_len) == LIB_ERROR) {
		return LIB_ERROR;
	}
	for (int i = 0; i < _data_len; i++) {
		pReceive_data[i] = _pData[i];
	}
	return LIB_OK;
}

LibReturnStateEnum Rs485::setSendDataWithCobsAndChecksum(const int *_pData,
		const int _data_len) {

	int pSend_data[SEND_DATA_LEN + 3] = { 0 };
	int pChecksum_data[SEND_DATA_LEN + 1] = { 0 };
	int send_len = 0;
	int crc8 = getCrc8(_pData, 0, _data_len - 1);
	for (int i = 0; i < _data_len; i++) {
		pChecksum_data[i] = _pData[i];
	}
	pChecksum_data[_data_len] = crc8;
	LibReturnStateEnum encode_flag = encodeCobs(&send_len, pSend_data,
			_data_len + 3, pChecksum_data, _data_len + 1);
	if (encode_flag == LIB_ERROR) {
		return LIB_ERROR;
	}
	return setSendData(pSend_data, send_len);
}

LibReturnStateEnum Rs485::sendData(const uint32_t _timeout) {
#ifdef ARDUINO_ARCH_SAM
	Time time;
	time.resetTimeout(_timeout);
	while(ds->txBufferIsAvailable() != 0){
		if (time.isTimeOut()) {
			return LIB_ERROR;
		}
	}
#endif
	for (int i = 0; i < getSendSize(); i++) {
		LibReturnStateEnum state = serialWrite(pSend_data[i]);
		if(state == LIB_ERROR){
			return LIB_ERROR;
		}
#ifdef DEBUG_RS485
    Serial.print(" ");
    Serial.print(pSend_data[i]);
  }
  Serial.println();
#else
	}
#endif
	clearSendData();
	return LIB_OK;
}

LibReturnStateEnum Rs485::serialWrite(const int _data) {
	if (_data > 0xFF || _data < 0) {
		return LIB_ERROR;
	}
#ifdef ARDUINO_ARCH_SAM
	uint8_t byte_data = (uint8_t) _data;
	ds->put(&byte_data, 1);
#else
  hs->write((uint8_t)_data);
#endif
	return LIB_OK;
}

int Rs485::serialRead(const int _timeout) {
	int data = 0;
	Time time;
	time.resetTimeout(_timeout);
	while (serialReadAvailable() <= 0) {
		if (time.isTimeOut()) {
			return INT_ERROR;
		}
	}
#ifdef ARDUINO_ARCH_SAM
	uint8_t byte_data = 0;
	ds->get(&byte_data, 1);
	data = byte_data;
#else
	data = (int)hs->read();
#endif
	if (data == -1) {
		return INT_ERROR;
	}
	return data;

}

LibReturnStateEnum Rs485::receiveDataWithCobsAndChecksum(
		const uint32_t _timeout) {
	if (serialReadAvailable() > 0) {
		int pReceive_data[RECEIVE_DATA_LEN + 3] = { 0 };
		int pDecode_data[RECEIVE_DATA_LEN] = { 0 };
		int receive_len = 0;
		int decode_len = 0;
		LibReturnStateEnum serial_flag = getSerialUntilZero(&receive_len,
				pReceive_data, RECEIVE_DATA_LEN + 3, _timeout);
		LibReturnStateEnum decode_flag = decodeData(&decode_len, pDecode_data,
				RECEIVE_DATA_LEN, pReceive_data, receive_len);
		if (serial_flag == LIB_ERROR || decode_flag == LIB_ERROR) {
			clearReceiveData();
			return LIB_ERROR;
		}
		setReceiveData(pDecode_data, decode_len);
	}
	return LIB_OK;
}

LibReturnStateEnum Rs485::getReceiveData(int *_receive_len, int *_pData,
		const int _data_len) {
	int receive_len = getReceiveSize();
	if (receive_len > _data_len) {
		*_receive_len = 0;
		return LIB_ERROR;
	}
	for (int i = 0; i < receive_len; i++) {
		_pData[i] = pReceive_data[i];
	}
	*_receive_len = receive_len;
}

LibReturnStateEnum Rs485::getSerialUntilZero(int *_receive_len, int *_pTarget,
		const int _target_len, const uint32_t _timeout) {
	int rcv_data = 1;
	clearLen(_pTarget, _target_len);
	for (*_receive_len = 0;
			*_receive_len < _target_len && rcv_data != 0
					&& rcv_data != INT_ERROR; (*_receive_len)++) {
		rcv_data = serialRead(_timeout);
		_pTarget[*_receive_len] = rcv_data;
	}
	if (rcv_data != 0) {
		clearLen(_pTarget, _target_len);
		return LIB_ERROR;
	}
	return LIB_OK;
}

int Rs485::serialReadAvailable(void) {
#ifdef ARDUINO_ARCH_SAM
	return ds->available();
#else
	return hs->available();
#endif
}
/////////////private////////////

LibReturnStateEnum Rs485::decodeData(int *pDecode_length, int *pTarget,
		const int target_length, int *pSource, const int source_length) { //todo:　引数をわかりやすくする
	if (source_length < 3) {
		return LIB_ERROR;
	}
	int pData[RECEIVE_DATA_LEN] = { 0 };
	for (int i = 0; i < source_length - 2; i++) {
		pData[i] = 0;
	}
	int data_length = 0;
	LibReturnStateEnum cobs_flag = decodeCobs(&data_length, pData,
			target_length, pSource, source_length);
	int checksum_pos = data_length - 1;
	if (getCrc8(pData, 0, checksum_pos - 1) != pData[checksum_pos]
			|| cobs_flag == LIB_ERROR) {
		return LIB_ERROR;
	}
	for (int i = 0; i < checksum_pos; i++) {
		pTarget[i] = pData[i];
	}
	*pDecode_length = checksum_pos;
	return LIB_OK;
}

int Rs485::getSendSize(void) {
	return send_size;
}

LibReturnStateEnum Rs485::setSendSize(const int _size) {
	if (_size > SEND_DATA_LEN) {
		send_size = 0;
		return LIB_ERROR;
	}
	send_size = _size;
	return LIB_OK;
}

int Rs485::getReceiveSize(void) {
	return receive_size;
}

LibReturnStateEnum Rs485::setReceiveSize(const int _size) {
	if (_size > RECEIVE_DATA_LEN) {
		receive_size = 0;
		return LIB_ERROR;
	}
	receive_size = _size;
	return LIB_OK;
}

LibBoolEnum Rs485::isByte(int *_pData, const int _data_len) {
	for (int i = 0; i < _data_len; i++) {
		if (_pData[i] > 0xFF || _pData[i] < 0) {
			return LIB_FALSE;
		}
	}
	return LIB_TRUE;
}

void Rs485::clearSendData(void) {
	setSendSize(0);
	for (int i = 0; i < SEND_DATA_LEN; i++) {
		pSend_data[i] = 0;
	}
}

void Rs485::clearReceiveData(void) {
	setReceiveSize(0);
	for (int i = 0; i < RECEIVE_DATA_LEN; i++) {
		pReceive_data[i] = 0;
	}
}

void Rs485::clearLen(int *_pData, const int _data_len) {
	for (int i = 0; i < _data_len; i++) {
		_pData[i] = 0;
	}
}

LibReturnStateEnum Rs485::decodeCobs(int *_pConvert_len, int *_pTarget,
		const int _target_len, int *_pSource, const int _source_len) {
	if (_source_len > _target_len + 2) {
		for (int i = 0; i < _target_len; i++) {
			_pTarget[i] = 0;
		}
		*_pConvert_len = 0;
		return LIB_ERROR;
	}
	int next_zero = _pSource[0];
	for (int i = 1; i < _source_len - 1; i++) {
		if (i == next_zero) {
			next_zero += _pSource[i];
			_pTarget[i - 1] = 0;
		} else {
			_pTarget[i - 1] = _pSource[i];
		}
	}
	*_pTarget = _source_len - 2;
	return LIB_OK;
}

LibReturnStateEnum Rs485::encodeCobs(int *_pConvert_len, int *_pTarget,
		const int _target_len, int *_pSource, const int _source_len) {
	if (_source_len + 2 > _target_len) {
		for (int i = 0; i < _target_len; i++) {
			_pTarget[i] = 0;
		}
		*_pConvert_len = 0;
		return LIB_ERROR;
	}
	for (int i = 0; i < _source_len; i++) {
		_pTarget[i + 1] = _pSource[i];
	}
	int i2 = 0;
	for (int i = _source_len; i >= 0; i--) {
		i2++;
		if (_pTarget[i] == 0) {
			_pTarget[i] = i2;
			i2 = 0;
		}
	}
	*_pConvert_len = _source_len + 2;
	return LIB_OK;

}

int Rs485::getCrc8(const int *s, const int start_pos, const int end_pos) {
	int crc8 = 0;
	for (int i = start_pos; i <= end_pos; i++) {
		crc8 = crc8_table[crc8 ^ (~s[i] & 0xFF)];
	}
	return crc8;
}


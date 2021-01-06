#ifndef SERIALPORT_H
#define SERIALPORT_H
#include <termios.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <cstring>
#include <iostream>
#include <unistd.h>

class SerialPort
{
private:
    int  PortDescriptor;
/*!
 * \brief Rodzaj kontroli parzystości
 *
 * Elementy typu określają rodzaj kontroli parzystości
 * dla transmisji szeregowej.
 */
enum Type4Parity {
  P_None  /*! Brak kontroli parzystości */,
  P_Even  /*! Pozytywna kontrola parzystości */,
  P_Odd   /*! Negatywna kontrola parzystości */,
  P_Mark  /*! Bit parzystości zawsze 1 */,
  P_Space /*! Bit parzystości zawsze 0 */
};


/*!
 * \brief Zestaw ustawień szybkości transmisji
 *
 * Typ zawiera zestaw możliwych prędkości transmisji szeregowej.
 */
enum Type4BaudRate {
    B_0 = B0,
    B_50 = B50,
    B_75 = B75,
    B_110 = B110,
    B_134 = B134,
    B_150 = B150,
    B_200 = B200,
    B_300 = B300,
    B_600 = B600,
    B_1200 = B1200,
    B_1800 = B1800,
    B_2400 = B2400,
    B_4800 = B4800,
    B_9600 = B9600,
    B_19200 = B19200,
    B_38400 = B38400,
    B_57600 = B57600,
    B_115200 = B115200,
    B_230400 = B230400
};


/*!
 * \brief Pozwala na ustawienie rozmiaru transmitowanego znaku
 *
 *  Pozwala na ustawienie rozmiaru transmitowanego znaku.
 */
enum Type4CharacterSize {
  CS_5 = CS5 /*! rozmiar znaku - 5 bitów */,
  CS_6 = CS6 /*! rozmiar znaku - 6 bitów */,
  CS_7 = CS7 /*! rozmiar znaku - 7 bitów */,
  CS_8 = CS8 /*! rozmiar znaku - 8 bitów */
};

/*!
 * \brief Ustawianie parametrów transmisji
 */

public:
    bool RS232_Send(int PortDescriptor, const uint8_t *Data);
    void ConfiguratePort(const char *sDevName);
    bool SetTransParam(int PortDesc, Type4BaudRate BRate  = B_115200, Type4Parity Parity = P_None, Type4CharacterSize CSize = CS_8);
    bool RS232_Nadaj(int DeskPortu, const char*  wNapis);
    void Send(uint8_t *Val);
    void SetTransParam_LocalModes( struct termios *pTransParam);
    void SetTransParam_InputModes( struct termios *pTransParam, Type4Parity Parity);
    void SetTransParam_OutputModes(struct termios *pTransParam);
    void SetTransParam_ControlModes(struct termios *pTransParam, Type4Parity Parity,Type4CharacterSize CSize);
    SerialPort();
};

#endif // SERIALPORT_H

#ifndef mbedSerial_H
#define mbedSerial_H

#include "mbed.h"
#include "WString.h"
#include <cstdio>
#include <cstring>
#include "drivers/SerialBase.h"

#define DEC 10
#define HEX 16
#define OCT 8
#ifdef BIN
#undef BIN
#endif
#define BIN 2

class MbedSerial
{
    public:

        #ifdef USBTX
        #ifdef USBRX

        MbedSerial(): ser(USBTX, USBRX) {isBegin = false;}

        #endif
        #endif
        
        MbedSerial(PinName tx, PinName rx): ser(tx, rx) {isBegin = false;}

        void begin(int baudRate) 
        {
            if (isBegin) return;

            ser.set_blocking(false);
            ser.set_baud(baudRate);
            isBegin = true;
        }

        
        void begin(int baudRate, int bits, mbed::SerialBase::Parity parity, int stop_bits) 
        {
            if (isBegin) return;

            ser.set_blocking(false);
            ser.set_baud(baudRate);
            ser.set_format(bits, parity, stop_bits);
            isBegin = true;
        }

        void println(String str) {__write(str + "\r\n");}
        void println(char c) {__write(String(c) + "\r\n");}
        void println(unsigned char c, unsigned char base = DEC) {__write(String(c, base) + "\r\n");}
        void println(int i, unsigned char base = DEC) {__write(String(i, base) + "\r\n");}
        void println(unsigned int i, unsigned char base = DEC) {__write(String(i, base) + "\r\n");}
        void println(long l, unsigned char base = DEC) {__write(String(l, base) + "\r\n");}
        void println(unsigned long l, unsigned char base = DEC) {__write(String(l, base) + "\r\n");}
        void println(float f, unsigned char decimalPlaces = 2) {__write((f < 10)? "0" : "" + String(f, decimalPlaces) + "\r\n");}
        void println(double d, unsigned char decimalPlaces = 2) {__write((d < 10)? "0" : "" + String(d, decimalPlaces) + "\r\n");}
        void println(void) {__write("\r\n");}

        void print(String str) {__write(str);}
        void print(char c) {__write(String(c));}
        void print(unsigned char c, unsigned char base = 10) {__write(String(c, base));}
        void print(int i, unsigned char base = 10) {__write(String(i, base));}
        void print(unsigned int i, unsigned char base = 10) {__write(String(i, base));}
        void print(long l, unsigned char base = 10) {__write(String(l, base));}
        void print(unsigned long l, unsigned char base = 10) {__write(String(l, base));}
        void print(float f, unsigned char decimalPlaces = 2) {__write((f < 10)? "0" : "" + String(f, decimalPlaces));}
        void print(double d, unsigned char decimalPlaces = 2) {__write((d < 10)? "0" : "" + String(d, decimalPlaces));}

        ssize_t available()
        {
            read_status = ser.read(buf, 1);
            return read_status;
        }

        char read()
        {
            return buf[0];
        }

        String readString()
        {
            String temp = "";
            
            if (read_status > 0)
            {
                temp += buf[0];
                while (available() > 0) 
                {
                    temp += buf[0];
                }
            }
            read_status = 0;
            return temp;
        }

        String readString(bool limiter)
        {
            if (limiter)
            {
                String temp = "";
                bool wait = true;
                int cnt = 0;

                if (read_status > 0)
                {
                    if (buf[0] == '\r' || buf[0] == '\n') wait = false;
                    else temp += buf[0];
                    while (wait) 
                    {
                        if (cnt >= 10000)
                        {
                            temp = "";
                            break;
                        } 

                        while (available() > 0) 
                        {
                            if (buf[0] == '\r' || buf[0] == '\n') wait = false;
                            else temp += buf[0];
                        }

                        cnt++;
                        wait_us(10);
                    }
                }

                read_status = 0;
                return temp;
            } else return readString();
        }
        
    private:
        BufferedSerial ser;
        bool isBegin;
        char buf[1];
        ssize_t read_status;

        void __write(String str) {
            if (!isBegin) return;

            int n = str.length() + 1;
            char buf[n];

            strcpy(buf, str.c_str());

            ser.write(buf, n);
        }
};

#endif
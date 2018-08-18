#include <array>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <algorithm>
#include <iterator>
#include "StatsRegister.h"
#include "IOPorts.h"

class CPU
{
    private:
        //Registers
        unsigned char A;
        unsigned char B;
        unsigned char C;
        unsigned char D;
        unsigned char E;
        unsigned char H;
        unsigned char L;

        unsigned short PC;
        unsigned short SP;

        unsigned char opcode;

        StatsRegister flags;
        IOPorts ports;
        bool interruptsEnabled;

        //64KB of memory
        std::array<unsigned char, 0x10000> memory;
        


    public:
        CPU(ushort startPC);

        void init();
        int emulateCycle();
        void readIntoMem(std::string filename, ushort startAddr);
        void incPC();

        std::array<unsigned char, 0x10000>& getMemory();

        void handleIN();
        void handleOUT();
        void setP1Left(bool on);
        void setP1Right(bool on);
        void setP1Fire(bool on);
        void setP2Left(bool on);
        void setP2Right(bool on);
        void setP2Fire(bool on);
        void setP1Start(bool on);
        void setP2Start(bool on);
        void setCoin(bool on);

        void genInterrupt(int interruptNum);

        void debugPrint();
        void setByte(ushort addr, unsigned char data);

        ushort getPC();
        unsigned char getOpcode();
        unsigned char getByte(ushort address);

        bool evenParity(unsigned char parityByte);
        bool getInterruptStatus();

        //instruction functions
        void nop();
        void lxiB();
        void lxiD();
        void lxiH();
        void lxiSP();
        void staxB();
        void staxD();
        void sta();
        void ldaxB();
        void ldaxD();
        void lda();
        void inxB();
        void inxD();
        void inxH();
        void inxSP();
        void dcxB();
        void dcxD();
        void dcxH();
        void dcxSP();
        void inrA();
        void inrB();
        void inrC();
        void inrD();
        void inrE();
        void inrH();
        void inrL();
        void inrM();
        void dcrA();
        void dcrB();
        void dcrC();
        void dcrD();
        void dcrE();
        void dcrH();
        void dcrL();
        void dcrM();
        void mviA();
        void mviB();
        void mviC();
        void mviD();
        void mviE();
        void mviH();
        void mviL();
        void mviM();
        void movBB();
        void movBC();
        void movBD();
        void movBE();
        void movBH();
        void movBL();
        void movBM();
        void movBA();
        void movCB();
        void movCC();
        void movCD();
        void movCE();
        void movCH();
        void movCL();
        void movCM();
        void movCA();
        void movDB();
        void movDC();
        void movDD();
        void movDE();
        void movDH();
        void movDL();
        void movDM();
        void movDA();
        void movEB();
        void movEC();
        void movED();
        void movEE();
        void movEH();
        void movEL();
        void movEM();
        void movEA();
        void movHB();
        void movHC();
        void movHD();
        void movHE();
        void movHH();
        void movHL();
        void movHM();
        void movHA();
        void movLB();
        void movLC();
        void movLD();
        void movLE();
        void movLH();
        void movLL();
        void movLM();
        void movLA();
        void movMB();
        void movMC();
        void movMD();
        void movME();
        void movMH();
        void movML();
        void movMA();
        void movAB();
        void movAC();
        void movAD();
        void movAE();
        void movAH();
        void movAL();
        void movAM();
        void movAA();
        void addB();
        void addC();
        void addD();
        void addE();
        void addH();
        void addL();
        void addM();
        void addA();
        void adi();
        void ani();
        void ori();
        void xri();
        void sui();
        void sbi();
        void aci();
        void adcB();
        void adcC();
        void adcD();
        void adcE();
        void adcH();
        void adcL();
        void adcM();
        void adcA();
        void subB();
        void subC();
        void subD();
        void subE();
        void subH();
        void subL();
        void subM();
        void subA();
        void sbbB();
        void sbbC();
        void sbbD();
        void sbbE();
        void sbbH();
        void sbbL();
        void sbbM();
        void sbbA();
        void anaB();
        void anaC();
        void anaD();
        void anaE();
        void anaH();
        void anaL();
        void anaM();
        void anaA();
        void xraB();
        void xraC();
        void xraD();
        void xraE();
        void xraH();
        void xraL();
        void xraM();
        void xraA();
        void oraB();
        void oraC();
        void oraD();
        void oraE();
        void oraH();
        void oraL();
        void oraM();
        void oraA();
        void cmpB();
        void cmpC();
        void cmpD();
        void cmpE();
        void cmpH();
        void cmpL();
        void cmpM();
        void cmpA();
        void rnz();
        void rz();
        void popB();
        void popD();
        void popH();
        void popPSW();
        void pushB();
        void pushD();
        void pushH();
        void pushPSW();
        void jnz();
        void jnc();
        void jpo();
        void jp();
        void jpe();
        void jc();
        void jz();
        void jm();
        void jmp();
        void cnz();
        void cnc();
        void cc();
        void cz();
        void call();
        void rst0();
        void rst1();
        void rst2();
        void rst3();
        void rst4();
        void rst5();
        void rst6();
        void rst7();
        void rpo();
        void rp();
        void rpe();
        void cpo();
        void cpe();
        void cp();
        void cpi();
        void cm();
        void ret();
        void rlc();
        void rrc();
        void dadB();
        void dadD();
        void dadH();
        void dadSP();
        void ral();
        void rar();
        void shld();
        void lhld();
        void cma();
        void cmc();
        void ctc();
        void rnc();
        void rc();
        void rm();
        void xthl();
        void pchl();
        void xchg();
        void sphl();
        void ei();
        void di();
};

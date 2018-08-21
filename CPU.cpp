#include "CPU.h"

using namespace std;

CPU::CPU(ushort startPC)
{
    init();
    this->PC = startPC;
}

void CPU::init()
{
    //reset registers
    this->A = 0;
    this->B = 0;
    this->C = 0;
    this->D = 0;
    this->E = 0;
    this->H = 0;
    this->L = 0;

    //reset PC and SP
    this->PC = 0;
    this->SP = 0;

    this->opcode = 0;

    //clear flags
    this->flags.S = 0;
    this->flags.C = 0;
    this->flags.Z = 0;
    this->flags.P = 0;
    this->interruptsEnabled = true;
    
    this->printDebugInfo = false;
    //clear memory
    for (uint i = 0; i < memory.size(); i++)
    {
        memory[i] = 0;
    }
}

void CPU::copyROM()
{
    for (int i = 0; i < 0x2000; i++)
    {
        ROMCopy[i] = memory[i];
    }
}
void CPU::readIntoMem(string filename, ushort startAddr)
{
    ifstream ROMFS(filename);

    std::copy(istreambuf_iterator<char>(ROMFS),
              istreambuf_iterator<char>(),
              memory.begin() + startAddr);
}

void CPU::setByte(ushort addr, unsigned char data)
{
    memory[addr] = data;
}

void CPU::debugPrint()
{
    cout << "PC = " << setfill('0') << setw(4) << hex << PC << endl;
    cout << "SP = " << setfill('0') << setw(4) << hex << (int)SP << endl << endl;
    cout << "OP = " << setfill('0') << setw(2) << hex << (int)opcode << endl << endl;
    cout << "A  = " << setfill('0') << setw(2) << hex << (int)A << endl;
    cout << "B  = " << setfill('0') << setw(2) << hex << (int)B << endl;
    cout << "C  = " << setfill('0') << setw(2) << hex << (int)C << endl;
    cout << "D  = " << setfill('0') << setw(2) << hex << (int)D << endl;
    cout << "E  = " << setfill('0') << setw(2) << hex << (int)E << endl;
    cout << "H  = " << setfill('0') << setw(2) << hex << (int)H << endl;
    cout << "L  = " << setfill('0') << setw(2) << hex << (int)L << endl << endl;
    cout << "S  = " << setfill('0') << setw(2) << hex << (int)flags.S << endl;
    cout << "C  = " << setfill('0') << setw(2) << hex << (int)flags.C << endl;
    cout << "Z  = " << setfill('0') << setw(2) << hex << (int)flags.Z << endl;
    cout << "P  = " << setfill('0') << setw(2) << hex << (int)flags.P << endl << endl;
    cout << "Read1 = " << setfill('0') << setw(2) << hex << (int)ports.read1 << endl;
    cout << "Read2 = " << setfill('0') << setw(2) << hex << (int)ports.read2 << endl;
    cout << "--------------------" << endl;

}

void CPU::writeMem(ushort addr, unsigned char value)
{
    if (addr < 0x2000) 
    {
        cout << "WARNING: write to ROM (addr = " << setfill('0') << setw(4) << hex << addr << ", PC = " 
             << setfill('0') << setw(4) << PC << ")" << endl;
        return;
    }
    if (addr >= 0x4000)
    {
        cout << "WARNING: write outside of RAM(addr = " << setfill('0') << setw(4) << hex << addr << ", PC = " 
             << setfill('0') << setw(4) << PC << ")" << endl;
        return;
    }
    memory[addr] = value;
}

//potential debug methods:
//  SplashSprite
//  EraseShifted
//  AnimateAlien (0x01be)
int CPU::emulateCycle()
{
    if (printDebugInfo)
    {
        cout << "PC = "  << setfill('0') << setw(4) << hex << PC 
             << "\tA = " << setfill('0') << setw(2) << hex << (int)A << endl;
    }

    opcode = memory[PC];
    int cycles = 0;

    switch (opcode)
    {
        case 0x00:
            nop();
            cycles = 4;
            break;

        case 0x01:
            lxiB();
            cycles = 10;
            break;

        case 0x02:
            staxB();
            cycles = 7;
            break;

        case 0x03:
            inxB();
            cycles = 5;
            break;

        case 0x04:
            inrB();
            cycles = 5;
            break;

        case 0x05: 
            dcrB();
            cycles = 5;
            break;

        case 0x06: 
            mviB();
            cycles = 7;
            break;

        case 0x07: 
            rlc();
            cycles = 4;
            break;

        case 0x08: 
            nop();
            cycles = 4;
            break;

        case 0x09: 
            dadB();
            cycles = 10;
            break;

        case 0x0a: 
            ldaxB();
            cycles = 7;
            break;

        case 0x0b: 
            dcxB();
            cycles = 7;
            break;

        case 0x0c: 
            inrC();
            cycles = 5;
            break;

        case 0x0d: 
            dcrC();
            cycles = 5;
            break;

        case 0x0e: 
            mviC();
            cycles = 7;
            break;

        case 0x0f: 
            rrc();
            cycles = 4;
            break;

        case 0x10: 
            nop();
            cycles = 4;
            break;

        case 0x11: 
            lxiD();
            cycles = 10;
            break;
            
        case 0x12: 
            staxD();
            cycles = 7;
            break;

        case 0x13: 
            inxD();
            cycles = 5;
            break;

        case 0x14: 
            inrD();
            cycles = 5;
            break;

        case 0x15: 
            dcrD();
            cycles = 5;
            break;

        case 0x16: 
            mviD();
            cycles = 7;
            break;

        case 0x17: 
            ral();
            cycles = 4;
            break;

        case 0x18: 
            nop();
            cycles = 4;
            break;

        case 0x19: 
            dadD();
            cycles = 10;
            break;

        case 0x1a: 
            ldaxD();
            cycles = 7;
            break;

        case 0x1b: 
            dcxD();
            cycles = 5;
            break;

        case 0x1c: 
            inrE();
            cycles = 5;
            break;

        case 0x1d: 
            dcrE();
            cycles = 5;
            break;

        case 0x1e: 
            mviE();
            cycles = 7;
            break;

        case 0x1f: 
            rar();
            cycles = 4;
            break;

        case 0x20: 
            nop();
            cycles = 4;
            break;

        case 0x21: 
            lxiH();
            cycles = 10;
            break;

        case 0x22: 
            shld();
            cycles = 16;
            break;

        case 0x23: 
            inxH();
            cycles = 5;
            break;

        case 0x24: 
            inrH();
            cycles = 5;
            break;

        case 0x25: 
            dcrH();
            cycles = 5;
            break;

        case 0x26: 
            mviH();
            cycles = 7;
            break;

        //DAA instruction (implement later?)
        case 0x27: 
            daa();
            cycles = 4;
            break;

        case 0x28: 
            nop();
            cycles = 4;
            break;

        case 0x29: 
            dadH();
            cycles = 10;
            break;

        case 0x2a: 
            lhld();
            cycles = 16;
            break;

        case 0x2b: 
            dcxH();
            cycles = 5;
            break;

        case 0x2c: 
            inrL();
            cycles = 5;
            break;

        case 0x2d: 
            dcrL();
            cycles = 5;
            break;

        case 0x2e: 
            mviL();
            cycles = 7;
            break;

        case 0x2f: 
            cma();
            cycles = 4;
            break;

        case 0x30: 
            nop();
            cycles = 4;
            break;

        case 0x31: 
            lxiSP();
            cycles = 10;
            break;

        case 0x32: 
            sta();
            cycles = 13;
            break;

        case 0x33: 
            inxSP();
            cycles = 5;
            break;

        case 0x34: 
            inrM();
            cycles = 10;
            break;

        case 0x35: 
            dcrM();
            cycles = 10;
            break;

        case 0x36: 
            mviM();
            cycles = 10;
            break;

        case 0x37: 
            ctc();
            cycles = 4;
            break;

        case 0x38: 
            nop();
            cycles = 4;
            break;

        case 0x39: 
            dadSP();
            cycles = 10;
            break;

        case 0x3a: 
            lda();
            cycles = 13;
            break;

        case 0x3b: 
            dcxSP();
            cycles = 5;
            break;

        case 0x3c: 
            inrA();
            cycles = 5;
            break;

        case 0x3d: 
            dcrA();
            cycles = 5;
            break;

        case 0x3e: 
            mviA();
            cycles = 7;
            break;

        case 0x3f: 
            cmc();
            cycles = 4;
            break;

        case 0x40: 
            movBB();
            cycles = 5;
            break;

        case 0x41: 
            movBC();
            cycles = 5;
            break;

        case 0x42: 
            movBD();
            cycles = 5;
            break;

        case 0x43: 
            movBE();
            cycles = 5;
            break;

        case 0x44: 
            movBH();
            cycles = 5;
            break;

        case 0x45: 
            movBL();
            cycles = 5;
            break;

        case 0x46: 
            movBM();
            cycles = 5;
            break;

        case 0x47: 
            movBA();
            cycles = 5;
            break;

        case 0x48: 
            movCB();
            cycles = 5;
            break;

        case 0x49: 
            movCC();
            cycles = 5;
            break;

        case 0x4a: 
            movCD();
            cycles = 5;
            break;

        case 0x4b: 
            movCE();
            cycles = 5;
            break;

        case 0x4c: 
            movCH();
            cycles = 5;
            break;

        case 0x4d: 
            movCL();
            cycles = 5;
            break;

        case 0x4e: 
            movCM();
            cycles = 5;
            break;

        case 0x4f: 
            movCA();
            cycles = 5;
            break;

        case 0x50: 
            movDB();
            cycles = 5;
            break;

        case 0x51: 
            movDC();
            cycles = 5;
            break;

        case 0x52: 
            movDD();
            cycles = 5;
            break;

        case 0x53: 
            movDE();
            cycles = 5;
            break;

        case 0x54: 
            movDH();
            cycles = 5;
            break;
            
        case 0x55: 
            movDL();
            cycles = 5;
            break;

        case 0x56: 
            movDM();
            cycles = 5;
            break;

        case 0x57: 
            movDA();
            cycles = 5;
            break;

        case 0x58: 
            movEB();
            cycles = 5;
            break;

        case 0x59: 
            movEC();
            cycles = 5;
            break;

        case 0x5a: 
            movED();
            cycles = 5;
            break;

        case 0x5b: 
            movEE();
            cycles = 5;
            break;

        case 0x5c: 
            movEH();
            cycles = 5;
            break;

        case 0x5d: 
            movEL();
            cycles = 5;
            break;

        case 0x5e: 
            movEM();
            cycles = 5;
            break;

        case 0x5f: 
            movEA();
            cycles = 5;
            break;

        case 0x60: 
            movHB();
            cycles = 5;
            break;

        case 0x61: 
            movHC();
            cycles = 5;
            break;

        case 0x62: 
            movHD();
            cycles = 5;
            break;

        case 0x63: 
            movHE();
            cycles = 5;
            break;

        case 0x64: 
            movHH();
            cycles = 5;
            break;

        case 0x65: 
            movHL();
            cycles = 5;
            break;

        case 0x66: 
            movHM();
            cycles = 5;
            break;

        case 0x67: 
            movHA();
            cycles = 5;
            break;

        case 0x68: 
            movLB();
            cycles = 5;
            break;

        case 0x69: 
            movLC();
            cycles = 5;
            break;

        case 0x6a: 
            movLD();
            cycles = 5;
            break;

        case 0x6b: 
            movLE();
            cycles = 5;
            break;

        case 0x6c: 
            movLH();
            cycles = 5;
            break;

        case 0x6d: 
            movLL();
            cycles = 5;
            break;

        case 0x6e: 
            movLM();
            cycles = 5;
            break;

        case 0x6f: 
            movLA();
            cycles = 5;
            break;

        case 0x70: 
            movMB();
            cycles = 5;
            break;

        case 0x71: 
            movMC();
            cycles = 5;
            break;

        case 0x72: 
            movMD();
            cycles = 5;
            break;

        case 0x73: 
            movME();
            cycles = 5;
            break;

        case 0x74: 
            movMH();
            cycles = 5;
            break;

        case 0x75: 
            movML();
            cycles = 5;
            break;

        case 0x76: 
            nop();
            cycles = 4;
            break;

        case 0x77: 
            movMA();
            cycles = 5;
            break;

        case 0x78: 
            movAB();
            cycles = 5;
            break;
            
        case 0x79: 
            movAC();
            cycles = 5;
            break;

        case 0x7a: 
            movAD();
            cycles = 5;
            break;

        case 0x7b: 
            movAE();
            cycles = 5;
            break;

        case 0x7c: 
            movAH();
            cycles = 5;
            break;

        case 0x7d: 
            movAL();
            cycles = 5;
            break;

        case 0x7e: 
            movAM();
            cycles = 5;
            break;

        case 0x7f: 
            movAA();
            cycles = 5;
            break;

        case 0x80: 
            addB();
            cycles = 4;
            break;

        case 0x81: 
            addC();
            cycles = 4;
            break;

        case 0x82: 
            addD();
            cycles = 4;
            break;

        case 0x83: 
            addE();
            cycles = 4;
            break;

        case 0x84: 
            addH();
            cycles = 4;
            break;

        case 0x85: 
            addL();
            cycles = 4;
            break;

        case 0x86: 
            addM();
            cycles = 4;
            break;

        case 0x87: 
            addA();
            cycles = 4;
            break;

        case 0x88: 
            adcB();
            cycles = 4;
            break;

        case 0x89: 
            adcC();
            cycles = 4;
            break;

        case 0x8a: 
            adcD();
            cycles = 4;
            break;

        case 0x8b: 
            adcE();
            cycles = 4;
            break;

        case 0x8c: 
            adcH();
            cycles = 4;
            break;

        case 0x8d: 
            adcL();
            cycles = 4;
            break;

        case 0x8e: 
            adcM();
            cycles = 4;
            break;

        case 0x8f: 
            adcA();
            cycles = 4;
            break;

        case 0x90: 
            subB();
            cycles = 4;
            break;

        case 0x91: 
            subC();
            cycles = 4;
            break;

        case 0x92: 
            subD();
            cycles = 4;
            break;

        case 0x93: 
            subE();
            cycles = 4;
            break;

        case 0x94: 
            subH();
            cycles = 4;
            break;

        case 0x95: 
            subL();
            cycles = 4;
            break;

        case 0x96: 
            subM();
            cycles = 4;
            break;

        case 0x97: 
            subA();
            cycles = 4;
            break;

        case 0x98: 
            sbbB();
            cycles = 4;
            break;

        case 0x99: 
            sbbC();
            cycles = 4;
            break;

        case 0x9a: 
            sbbD();
            cycles = 4;
            break;

        case 0x9b: 
            sbbE();
            cycles = 4;
            break;

        case 0x9c: 
            sbbH();
            cycles = 4;
            break;

        case 0x9d: 
            sbbL();
            cycles = 4;
            break;

        case 0x9e: 
            sbbM();
            cycles = 4;
            break;

        case 0x9f: 
            sbbA();
            cycles = 4;
            break;

        case 0xa0: 
            anaB();
            cycles = 4;
            break;

        case 0xa1: 
            anaC();
            cycles = 4;
            break;

        case 0xa2: 
            anaD();
            cycles = 4;
            break;

        case 0xa3: 
            anaE();
            cycles = 4;
            break;

        case 0xa4: 
            anaH();
            cycles = 4;
            break;

        case 0xa5: 
            anaL();
            cycles = 4;
            break;

        case 0xa6: 
            anaM();
            cycles = 4;
            break;

        case 0xa7: 
            anaA();
            cycles = 4;
            break;

        case 0xa8: 
            xraB();
            cycles = 4;
            break;

        case 0xa9: 
            xraC();
            cycles = 4;
            break;

        case 0xaa: 
            xraD();
            cycles = 4;
            break;

        case 0xab: 
            xraE();
            cycles = 4;
            break;

        case 0xac: 
            xraH();
            cycles = 4;
            break;

        case 0xad: 
            xraL();
            cycles = 4;
            break;

        case 0xae: 
            xraM();
            cycles = 4;
            break;

        case 0xaf: 
            xraA();
            cycles = 4;
            break;

        case 0xb0: 
            oraB();
            cycles = 4;
            break;

        case 0xb1: 
            oraC();
            cycles = 4;
            break;

        case 0xb2: 
            oraD();
            cycles = 4;
            break;

        case 0xb3: 
            oraE();
            cycles = 4;
            break;

        case 0xb4: 
            oraH();
            cycles = 4;
            break;

        case 0xb5: 
            oraL();
            cycles = 4;
            break;

        case 0xb6: 
            oraM();
            cycles = 4;
            break;

        case 0xb7: 
            oraA();
            cycles = 4;
            break;

        case 0xb8: 
            cmpB();
            cycles = 4;
            break;

        case 0xb9: 
            cmpC();
            cycles = 4;
            break;

        case 0xba: 
            cmpD();
            cycles = 4;
            break;

        case 0xbb: 
            cmpE();
            cycles = 4;
            break;

        case 0xbc: 
            cmpH();
            cycles = 4;
            break;

        case 0xbd: 
            cmpL();
            cycles = 4;
            break;

        case 0xbe: 
            cmpM();
            cycles = 4;
            break;

        case 0xbf: 
            cmpA();
            cycles = 4;
            break;

        case 0xc0: 
            rnz();
            cycles = 7;
            break;

        case 0xc1: 
            popB();
            cycles = 10;
            break;

        case 0xc2: 
            jnz();
            cycles = 10;
            break;

        case 0xc3: 
            jmp();
            cycles = 10;
            break;

        case 0xc4: 
            cnz();
            cycles = 14;
            break;

        case 0xc5: 
            pushB();
            cycles = 11;
            break;

        case 0xc6: 
            adi();
            cycles = 7;
            break;

        case 0xc7: 
            rst0();
            cycles = 11;
            break;

        case 0xc8: 
            rz();
            cycles = 7;
            break;

        case 0xc9: 
            ret();
            cycles = 10;
            break;

        case 0xca: 
            jz();
            cycles = 10;
            break;

        case 0xcb: 
            nop();
            cycles = 4;
            break;

        case 0xcc: 
            cz();
            cycles = 14;
            break;

        case 0xcd: 
            call();
            cycles = 17;
            break;

        case 0xce: 
            aci();
            cycles = 7;
            break;

        case 0xcf: 
            rst1();
            cycles = 11;
            break;

        case 0xd0: 
            rnc();
            cycles = 7;
            break;

        case 0xd1: 
            popD();
            cycles = 10;
            break;

        case 0xd2: 
            jnc();
            cycles = 10;
            break;

        case 0xd3: 
            handleOUT();
            cycles = 10;
            break;

        case 0xd4: 
            cnc();
            cycles = 14;
            break;

        case 0xd5: 
            pushD();
            cycles = 11;
            break;

        case 0xd6: 
            sui();
            cycles = 7;
            break;

        case 0xd7: 
            rst2();
            cycles = 11;
            break;

        case 0xd8: 
            rc();
            cycles = 7;
            break;

        case 0xd9: 
            nop();
            cycles = 4;
            break;

        case 0xda: 
            jc();
            cycles = 10;
            break;

        case 0xdb: 
            handleIN();
            cycles = 10;
            break;

        case 0xdc: 
            cc();
            cycles = 14;
            break;

        case 0xdd: 
            nop();
            cycles = 4;
            break;

        case 0xde: 
            sbi();
            cycles = 7;
            break;

        case 0xdf: 
            rst3();
            cycles = 11;
            break;

        case 0xe0: 
            rpo();
            cycles = 7;
            break;

        case 0xe1: 
            popH();
            cycles = 10;
            break;

        case 0xe2: 
            jpo();
            cycles = 10;
            break;

        case 0xe3: 
            xthl();
            cycles = 18;
            break;

        case 0xe4: 
            cpo();
            cycles = 14;
            break;

        case 0xe5: 
            pushH();
            cycles = 11;
            break;

        case 0xe6: 
            ani();
            cycles = 7;
            break;

        case 0xe7: 
            rst4();
            cycles = 11;
            break;

        case 0xe8: 
            rpe();
            cycles = 7;
            break;

        case 0xe9: 
            pchl();
            cycles = 5;
            break;

        case 0xea: 
            jpe();
            cycles = 10;
            break;

        case 0xeb: 
            xchg();
            cycles = 5;
            break;

        case 0xec: 
            cpe();
            cycles = 14;
            break;

        case 0xed: 
            nop();
            cycles = 4;
            break;

        case 0xee: 
            xri();
            cycles = 7;
            break;

        case 0xef: 
            rst5();
            cycles = 11;
            break;

        case 0xf0: 
            rp();
            cycles = 7;
            break;

        case 0xf1: 
            popPSW();
            cycles = 10;
            break;

        case 0xf2: 
            jp();
            cycles = 10;
            break;

        case 0xf3: 
            di();
            cycles = 4;
            break;

        case 0xf4: 
            cp();
            cycles = 14;
            break;

        case 0xf5: 
            pushPSW();
            cycles = 11;
            break;

        case 0xf6: 
            ori();
            cycles = 7;
            break;

        case 0xf7: 
            rst6();
            cycles = 11;
            break;

        case 0xf8: 
            rm();
            cycles = 11;
            break;

        case 0xf9: 
            sphl();
            cycles = 5;
            break;

        case 0xfa: 
            jm();
            cycles = 10;
            break;

        case 0xfb: 
            ei();
            cycles = 4;
            break;

        case 0xfc: 
            cm();
            cycles = 14;
            break;

        case 0xfd: 
            nop();
            cycles = 4;
            break;

        case 0xfe: 
            cpi();
            cycles = 7;
            break;

        case 0xff: 
            rst7();
            cycles = 11;
            break;
    }

    return cycles;
}

ushort CPU::getPC()
{
    return PC;
}

array<unsigned char, 0x10000>& CPU::getMemory()
{
    return memory;
}

unsigned char CPU::getOpcode()
{
    return memory[PC];
}

unsigned char CPU::getByte(ushort address)
{
    return memory[address];
}

bool CPU::getInterruptStatus()
{
    return this->interruptsEnabled;
}

bool CPU::ROMCorrupted()
{
    for (int i = 0; i < ROMCopy.size(); i++)
    {
        if (memory[i] != ROMCopy[i])
        {
            return true;
        }
    }
    return false;
}

bool CPU::evenParity(unsigned char parityByte)
{
    int setCount = 0;
    for (int i = 0; i < 8; i++)
    {
        if (parityByte & 0x01)
        {
            setCount++;
        }
        parityByte = parityByte >> 1;
    }
    return (setCount % 2 == 0);
}

void CPU::genInterrupt(int interruptNum)
{
    //push PC on stack
    writeMem(SP - 2, PC & 0xff);
    writeMem(SP - 1, (PC & 0xff00) >> 8);
    SP -= 2;

    //set PC to interrupt vector
    PC = 8 * interruptNum;

    //mimic DI
    interruptsEnabled = false;
}

void CPU::setDebug(bool debug)
{
    printDebugInfo = debug;
}
void CPU::setP1Left(bool on)
{
    if (on)
    {
        //set bit 5 (P1 left) 
        ports.read1 |= 0x20;
    }
    else
    {
        //clear bit 5 (P1 left)
        ports.read1 &= 0xDF;
    }
}


void CPU::setP2Left(bool on)
{
    if (on)
    {
        //set bit 5 (P1 left) 
        ports.read2 |= 0x20;
    }
    else
    {
        //clear bit 5 (P1 left)
        ports.read2 &= 0xDF;
    }
}

void CPU::setP1Right(bool on)
{
    if (on)
    {
        //set bit 6 (P1 right) 
        ports.read1 |= 0x40;
    }
    else
    {
        //clear bit 6 (P1 right) 
        ports.read1 &= 0xBF;
    }
}

void CPU::setP2Right(bool on)
{
    if (on)
    {
        //set bit 6 (P1 right) 
        ports.read2 |= 0x40;
    }
    else
    {
        //clear bit 6 (P1 right) 
        ports.read2 &= 0xBF;
    }
}


void CPU::setP1Fire(bool on)
{
    if (on)
    {
        //set bit 4 (P1 fire) 
        ports.read1 |= 0x10;
    }
    else
    {
        //clear bit 4 (P1 fire) 
        ports.read1 &= 0xEF;
    }
}

void CPU::setP1Start(bool on)
{
    if (on)
    {
        //set bit 2 (P1 start) 
        ports.read1 |= 0x04;
    }
    else
    {
        //clear bit 2 (P1 start) 
        ports.read1 &= 0xFB;
    }
}

void CPU::setP2Start(bool on)
{
    if (on)
    {
        //set bit 1 (P2 start) 
        ports.read1 |= 0x02;
    }
    else
    {
        //clear bit 1 (P2 start) 
        ports.read1 &= 0xFD;
    }
}

void CPU::setCoin(bool on)
{
    if (on)
    {
        //set bit 0 (coin - 0 when active) 
        ports.read1 |= 0x01;
    }
    else
    {
        //clear bit 0 (coin)
        ports.read1 &= 0xFE;
    }
}

void CPU::handleIN()
{
    unsigned char port = memory[PC + 1];
    unsigned char result;

    switch (port)
    {
        case 0:
            result = 1;
            break;

        case 1:
            //inputs
            result = ports.read1;
            break;

        case 3:
            //bit shift register read
            ushort v = (ports.shift1 << 8) | ports.shift0;
            result = ((v >> (8 - ports.write2)) & 0xff);
            break;
    }
    A = result;
    PC += 2;
}

void CPU::handleOUT()
{
    unsigned char port = memory[PC + 1];

    switch (port)
    {
        case 2:
            //shift amount (3 bits)
            ports.write2 = A & 0x7;
            break;

        case 4:
            //shift data 
            ports.shift0 = ports.shift1;
            ports.shift1 = A;
            break;
    }

    PC += 2;
}

void CPU::incPC()
{
    PC += 1;
}

void CPU::nop()
{
    PC += 1;
}

void CPU::lxiB()
{
    unsigned char lowByte  = memory[PC + 1];
    unsigned char highByte = memory[PC + 2];

    B = highByte;
    C = lowByte;
    
    PC += 3;
}

void CPU::lxiD()
{
    unsigned char lowByte  = memory[PC + 1];
    unsigned char highByte = memory[PC + 2];

    D = highByte;
    E = lowByte;
    
    PC += 3;
}

void CPU::lxiH()
{
    unsigned char lowByte  = memory[PC + 1];
    unsigned char highByte = memory[PC + 2];

    H = highByte;
    L = lowByte;
    
    PC += 3;
}

void CPU::lxiSP()
{
    unsigned char lowByte  = memory[PC + 1];
    unsigned char highByte = memory[PC + 2];
    SP = (ushort)(highByte << 8) | lowByte;

    PC += 3;
}

void CPU::staxB()
{
    //get store addr from register pair BC
    ushort storeAddr = (ushort)(B << 8) | C;

    //store accumulator in mem at address
    writeMem(storeAddr, A);

    PC += 1;
}

void CPU::staxD()
{
    //get store addr from register pair BC
    ushort storeAddr = (ushort)(D << 8) | E;

    //store accumulator in mem at address
    writeMem(storeAddr, A);

    PC += 1;
}

void CPU::sta()
{
    ushort storeAddr = (ushort)(memory[PC + 2] << 8) | memory[PC + 1];
    writeMem(storeAddr, A);

    PC += 3;
}

void CPU::ldaxB()
{
    //get load addr from register pair BC
    ushort loadAddr = (ushort)(B << 8) | C;

    //load byte at address into accumulator
    A = memory[loadAddr];

    PC += 1;
}

void CPU::ldaxD()
{
    //get load addr from register pair DE
    ushort loadAddr = (ushort)(D << 8) | E;

    //load byte at address into accumulator
    A = memory[loadAddr];

    PC += 1;
}

void CPU::lda()
{
    ushort loadAddr = (ushort)(memory[PC + 2] << 8) | memory[PC + 1];
    A = memory[loadAddr];

    PC += 3;
}

void CPU::inxB()
{
    //fetch the value in register pair BC
    ushort BCValue = (ushort)(B << 8) | C;
    BCValue += 1;

    //store the upper and lower bytes of BC in registers B & C
    B = (unsigned char)((BCValue >> 8) & 0xff);
    C = (unsigned char)(BCValue & 0x00ff);

    PC += 1;
}

void CPU::inxD()
{
    //fetch the value in register pair DE
    ushort DEValue = (ushort)(D << 8) | E;
    DEValue += 1;

    //store the upper and lower bytes of BC in registers B & C
    D = (unsigned char)((DEValue >> 8) & 0xff);
    E = (unsigned char)(DEValue & 0x00ff);

    PC += 1;
}

void CPU::inxH()
{
    //fetch the value in register pair HL
    ushort HLValue = (ushort)(H << 8) | L;
    HLValue += 1;

    //store the upper and lower bytes of BC in registers B & C
    H = (unsigned char)((HLValue >> 8) & 0xff);
    L = (unsigned char)(HLValue & 0x00ff);

    PC += 1;
}

void CPU::inxSP()
{
    SP += 1;
    PC += 1;
}


void CPU::dcxB()
{
    //fetch the value in register pair BC
    ushort BCValue = (ushort)(B << 8) | C;
    BCValue -= 1;

    //store the upper and lower bytes of BC in registers B & C
    B = (unsigned char)((BCValue >> 8) & 0xff);
    C = (unsigned char)(BCValue & 0x00ff);

    PC += 1;
}

void CPU::dcxD()
{
    //fetch the value in register pair DE
    ushort DEValue = (ushort)(D << 8) | E;
    DEValue -= 1;

    //store the upper and lower bytes of BC in registers B & C
    D = (unsigned char)((DEValue >> 8) & 0xff);
    E = (unsigned char)(DEValue & 0x00ff);

    PC += 1;
}

void CPU::dcxH()
{
    //fetch the value in register pair HL
    ushort HLValue = (ushort)(H << 8) | L;
    HLValue -= 1;

    //store the upper and lower bytes of BC in registers B & C
    H = (unsigned char)((HLValue >> 8) & 0xff);
    L = (unsigned char)(HLValue & 0x00ff);

    PC += 1;
}

void CPU::dcxSP()
{
    SP -= 1;
    PC += 1;
}

void CPU::inrA()
{
    unsigned char result = A + 1;

    //set flags
    flags.Z = (result == 0);
    flags.S = ((result & 0x80) == 0x80);
    flags.P = evenParity(result);

    //update register
    A = result;
    PC += 1;
}

void CPU::inrB()
{
    unsigned char result = B + 1;

    //set flags
    flags.Z = (result == 0);
    flags.S = ((result & 0x80) == 0x80);
    flags.P = evenParity(result);

    //update register
    B = result;
    PC += 1;
}

void CPU::inrC()
{
    unsigned char result = C + 1;

    //set flags
    flags.Z = (result == 0);
    flags.S = ((result & 0x80) == 0x80);
    flags.P = evenParity(result);

    //update register
    C = result;
    PC += 1;
}

void CPU::inrD()
{
    unsigned char result = D + 1;

    //set flags
    flags.Z = (result == 0);
    flags.S = ((result & 0x80) == 0x80);
    flags.P = evenParity(result);

    //update register
    D = result;
    PC += 1;
}

void CPU::inrE()
{
    unsigned char result = E + 1;

    //set flags
    flags.Z = (result == 0);
    flags.S = ((result & 0x80) == 0x80);
    flags.P = evenParity(result);

    //update register
    E = result;
    PC += 1;
}

void CPU::inrH()
{
    unsigned char result = H + 1;

    //set flags
    flags.Z = (result == 0);
    flags.S = ((result & 0x80) == 0x80);
    flags.P = evenParity(result);

    //update register
    H = result;
    PC += 1;
}

void CPU::inrL()
{
    unsigned char result = L + 1;

    //set flags
    flags.Z = (result == 0);
    flags.S = ((result & 0x80) == 0x80);
    flags.P = evenParity(result);

    //update register
    L = result;
    PC += 1;
}

void CPU::inrM()
{
    //fetch address from HL and increment it
    ushort HLAddr = (ushort)(H << 8) | L;
    unsigned char result = memory[HLAddr] + 1;

    flags.Z = (result == 0);
    flags.S = ((result & 0x80) == 0x80);
    flags.P = evenParity(result);

    writeMem(HLAddr, result);
    PC += 1;
}

void CPU::dcrA()
{
    unsigned char result = A - 1;

    //set flags
    flags.Z = (result == 0);
    flags.S = ((result & 0x80) == 0x80);
    flags.P = evenParity(result);

    //update register
    A = result;
    PC += 1;
}
void CPU::dcrB()
{
    //compute result as 16 bit
    unsigned char result = B - 1;

    //set flags
    flags.Z = (result == 0);
    flags.S = ((result & 0x80) == 0x80);
    flags.P = evenParity(result);

    //update register
    B = result;
    PC += 1;
}

void CPU::dcrC()
{
    //compute result as 16 bit
    unsigned char result = C - 1;

    //set flags
    flags.Z = (result == 0);
    flags.S = ((result & 0x80) == 0x80);
    flags.P = evenParity(result);

    //update register
    C = result;
    PC += 1;
}

void CPU::dcrD()
{
    //compute result as 16 bit
    unsigned char result = D - 1;

    //set flags
    flags.Z = (result == 0);
    flags.S = ((result & 0x80) == 0x80);
    flags.P = evenParity(result);

    //update register
    D = result;
    PC += 1;
}

void CPU::dcrE()
{
    //compute result as 16 bit
    unsigned char result = E - 1;

    //set flags
    flags.Z = (result == 0);
    flags.S = ((result & 0x80) == 0x80);
    flags.P = evenParity(result);

    //update register
    E = result;
    PC += 1;
}

void CPU::dcrH()
{
    //compute result as 16 bit
    unsigned char result = H - 1;

    //set flags
    flags.Z = (result == 0);
    flags.S = ((result & 0x80) == 0x80);
    flags.P = evenParity(result);

    //update register
    H = result;
    PC += 1;
}

void CPU::dcrL()
{
    //compute result as 16 bit
    unsigned char result = L - 1;

    //set flags
    flags.Z = (result == 0);
    flags.S = ((result & 0x80) == 0x80);
    flags.P = evenParity(result);

    //update register
    L = result;
    PC += 1;
}

void CPU::dcrM()
{
    //fetch address from HL and increment it
    ushort HLAddr = (ushort)(H << 8) | L;
    unsigned char result = memory[HLAddr] - 1;

    flags.Z = (result == 0);
    flags.S = ((result & 0x80) == 0x80);
    flags.P = evenParity(result);

    writeMem(HLAddr, result);
    PC += 1;
}


void CPU::mviA()
{
    //fetch byte after opcode
    unsigned char immediateByte = memory[PC + 1];
    A = immediateByte;
    PC += 2;
}

void CPU::mviB()
{
    //fetch byte after opcode
    unsigned char immediateByte = memory[PC + 1];
    B = immediateByte;
    PC += 2;
}

void CPU::mviC()
{
    //fetch byte after opcode
    unsigned char immediateByte = memory[PC + 1];
    C = immediateByte;
    PC += 2;
}

void CPU::mviD()
{
    //fetch byte after opcode
    unsigned char immediateByte = memory[PC + 1];
    D = immediateByte;
    PC += 2;
}

void CPU::mviE()
{
    //fetch byte after opcode
    unsigned char immediateByte = memory[PC + 1];
    E = immediateByte;
    PC += 2;
}

void CPU::mviH()
{
    //fetch byte after opcode
    unsigned char immediateByte = memory[PC + 1];
    H = immediateByte;
    PC += 2;
}

void CPU::mviL()
{
    //fetch byte after opcode
    unsigned char immediateByte = memory[PC + 1];
    L = immediateByte;
    PC += 2;
}

void CPU::mviM()
{
    ushort HLAddr = (ushort)(H << 8) | L;
    unsigned char storeByte = memory[PC + 1];

    writeMem(HLAddr, storeByte);
    PC += 2;
}

void CPU::rlc()
{
    unsigned char prevA = A;

    //shift A left and rotate bits around
    A = ((prevA >> 7) | (prevA << 1));

    //set carry flag based on A's MSB before shift
    flags.C = ((prevA >> 7) == 1);
    PC += 1;
}

void CPU::rrc()
{
    unsigned char prevA = A;

    //shift A left and rotate bits around
    A = (((prevA & 0x1) << 7) | (prevA >> 1));

    //set carry flag based on A's MSB before shift
    flags.C = ((prevA & 0x1) == 1);
    PC += 1;
}

void CPU::rm()
{
    if (flags.S)
    {
        ret();
    }
    else
    {
        PC += 1;
    }
}

void CPU::ral()
{
    unsigned char prevA = A;

    //shift A left by one and set LSB to carry bit
    A = (prevA << 1) | (flags.C & 0x1);

    //set carry bit to MSB of A before shift
    flags.C = ((prevA & 0x80) == 0x80);
    PC += 1;
}

void CPU::rar()
{
    unsigned char prevA = A;

    //shift A right by one and set MSB to MSB of A before shift
    A = (flags.C << 7) | (prevA >> 1);

    //set carry bit to LSB of A before shift
    flags.C = ((prevA & 0x1) == 1);
    PC += 1;
}

void CPU::dadB()
{
    //interpret register pair as 32-bit value to capture carry
    int HLValue = (int)(H << 8) | L;
    int BCValue = (int)(B << 8) | C;

    int result = HLValue  + BCValue;
    //check for carry
    flags.C = (result > 0xffff);

    //assign H and L to high and low bytes of result
    H = (unsigned char)((result & 0xff00) >> 8);
    L = (unsigned char)(result & 0xff);

    PC += 1;
}

void CPU::dadD()
{
    //interpret register pair as 32-bit value to capture carry
    int HLValue = (int)(H << 8) | L;
    int DEValue = (int)(D << 8) | E;

    int result = HLValue  + DEValue;
    //check for carry
    flags.C = (result > 0xffff);

    //assign H and L to high and low bytes of result
    H = (unsigned char)((result & 0xff00) >> 8);
    L = (unsigned char)(result & 0xff);

    PC += 1;
}

void CPU::dadH()
{
    //interpret register pair as 32-bit value to capture carry
    int HLValue = (int)(H << 8) | L;

    int result = HLValue + HLValue;
    //check for carry
    flags.C = (result > 0xffff);

    //assign H and L to high and low bytes of result
    H = (unsigned char)((result & 0xff00) >> 8);
    L = (unsigned char)(result & 0xff);

    PC += 1;
}

void CPU::dadSP()
{
    //interpret register pair as 32-bit value to capture carry
    int HLValue = (int)(H << 8) | L;

    int result = HLValue + SP;
    //check for carry
    flags.C = (result > 0xffff);

    //assign H and L to high and low bytes of result
    H = (unsigned char)((result & 0xff00) >> 8);
    L = (unsigned char)(result & 0x000000ff);

    PC += 1;
}

void CPU::shld()
{
    //load address from immediate bytes in little-endian format
    ushort storeAddr = (ushort)(memory[PC + 2] << 8) | memory[PC + 1];
    writeMem(storeAddr, L);
    writeMem(storeAddr + 1, H);

    PC += 3;
}

void CPU::lhld()
{
    //load address from immediate bytes in little-endian format
    ushort loadAddr = (ushort)(memory[PC + 2] << 8) | memory[PC + 1];
    L = memory[loadAddr];
    H = memory[loadAddr + 1];

    PC += 3;
}

void CPU::cma()
{
    A = ~A;
    PC += 1;
}

void CPU::ctc()
{
    flags.C = 1;
    PC += 1;
}

void CPU::cmc()
{
    flags.C = !flags.C;
    PC += 1;
}

void CPU::cm()
{
    if (flags.S)
    {
        call();
    }
    else
    {
        PC += 3;
    }
}

void CPU::movBB()
{
    B = B;
    PC += 1;
}

void CPU::movBC()
{
    B = C;
    PC += 1;
}

void CPU::movBD()
{
    B = D;
    PC += 1;
}

void CPU::movBE()
{
    B = E;
    PC += 1;
}

void CPU::movBH()
{
    B = H;
    PC += 1;
}

void CPU::movBL()
{
    B = L;
    PC += 1;
}

void CPU::movBM()
{
    ushort HLAddr = (ushort)(H << 8) | L;
    B = memory[HLAddr];
    PC += 1;
}

void CPU::movBA()
{
    B = A;
    PC += 1;
}

void CPU::movCB()
{
    C = B;
    PC += 1;
}

void CPU::movCC()
{
    C = C;
    PC += 1;
}

void CPU::movCD()
{
    C = D;
    PC += 1;
}

void CPU::movCE()
{
    C = E;
    PC += 1;
}

void CPU::movCH()
{
    C = H;
    PC += 1;
}

void CPU::movCL()
{
    C = L;
    PC += 1;
}

void CPU::movCM()
{
    ushort HLAddr = (ushort)(H << 8) | L;
    C = memory[HLAddr];
    PC += 1;
}

void CPU::movCA()
{
    C = A;
    PC += 1;
}

void CPU::movDB()
{
    D = B;
    PC += 1;
}

void CPU::movDC()
{
    D = C;
    PC += 1;
}

void CPU::movDD()
{
    D = D;
    PC += 1;
}

void CPU::movDE()
{
    D = E;
    PC += 1;
}

void CPU::movDH()
{
    D = H;
    PC += 1;
}

void CPU::movDL()
{
    D = L;
    PC += 1;
}

void CPU::movDM()
{
    ushort HLAddr = (ushort)(H << 8) | L;
    D = memory[HLAddr];
    PC += 1;
}

void CPU::movDA()
{
    D = A;
    PC += 1;
}

void CPU::movEB()
{
    E = B;
    PC += 1;
}

void CPU::movEC()
{
    E = C;
    PC += 1;
}

void CPU::movED()
{
    E = D;
    PC += 1;
}

void CPU::movEE()
{
    E = E;
    PC += 1;
}

void CPU::movEH()
{
    E = H;
    PC += 1;
}

void CPU::movEL()
{
    E = L;
    PC += 1;
}

void CPU::movEM()
{
    ushort HLAddr = (ushort)(H << 8) | L;
    E = memory[HLAddr];
    PC += 1;
}

void CPU::movEA()
{
    E = A;
    PC += 1;
}

void CPU::movHB()
{
    H = B;
    PC += 1;
}

void CPU::movHC()
{
    H = C;
    PC += 1;
}

void CPU::movHD()
{
    H = D;
    PC += 1;
}

void CPU::movHE()
{
    H = E;
    PC += 1;
}

void CPU::movHH()
{
    H = H;
    PC += 1;
}

void CPU::movHL()
{
    H = L;
    PC += 1;
}

void CPU::movHM()
{
    ushort HLAddr = (ushort)(H << 8) | L;
    H = memory[HLAddr];
    PC += 1;
}

void CPU::movHA()
{
    H = A;
    PC += 1;
}

void CPU::movLB()
{
    L = B;
    PC += 1;
}

void CPU::movLC()
{
    L = C;
    PC += 1;
}

void CPU::movLD()
{
    L = D;
    PC += 1;
}

void CPU::movLE()
{
    L = E;
    PC += 1;
}

void CPU::movLH()
{
    L = H;
    PC += 1;
}

void CPU::movLL()
{
    L = L;
    PC += 1;
}

void CPU::movLM()
{
    ushort HLAddr = (ushort)(H << 8) | L;
    L = memory[HLAddr];
    PC += 1;
}

void CPU::movLA()
{
    L = A;
    PC += 1;
}

void CPU::movMB()
{
    ushort HLAddr = (ushort)(H << 8) | L;
    writeMem(HLAddr, B);
    PC += 1;
}

void CPU::movMC()
{
    ushort HLAddr = (ushort)(H << 8) | L;
    writeMem(HLAddr, C);
    PC += 1;
}

void CPU::movMD()
{
    ushort HLAddr = (ushort)(H << 8) | L;
    writeMem(HLAddr, D);
    PC += 1;
}

void CPU::movME()
{
    ushort HLAddr = (ushort)(H << 8) | L;
    writeMem(HLAddr, E);
    PC += 1;
}

void CPU::movMH()
{
    ushort HLAddr = (ushort)(H << 8) | L;
    writeMem(HLAddr, H);
    PC += 1;
}

void CPU::movML()
{
    ushort HLAddr = (ushort)(H << 8) | L;
    writeMem(HLAddr, L);
    PC += 1;
}

void CPU::movMA()
{
    ushort HLAddr = (ushort)(H << 8) | L;
    writeMem(HLAddr, A);
    PC += 1;
}

void CPU::movAB()
{
    A = B;
    PC += 1;
}

void CPU::movAC()
{
    A = C;
    PC += 1;
}

void CPU::movAD()
{
    A = D;
    PC += 1;
}

void CPU::movAE()
{
    A = E;
    PC += 1;
}

void CPU::movAH()
{
    A = H;
    PC += 1;
}

void CPU::movAL()
{
    A = L;
    PC += 1;
}

void CPU::movAM()
{
    ushort HLAddr = (ushort)(H << 8) | L;
    A = memory[HLAddr];
    PC += 1;
}

void CPU::movAA()
{
    A = A;
    PC += 1;
}

void CPU::addB()
{
    ushort result = (ushort)A + (ushort)B;
    flags.Z = ((result & 0xff) == 0);
    flags.S = ((result & 0x80) == 0x80);
    flags.P = evenParity(result);
    flags.C = (result > 0xff);

    A = (unsigned char)(result & 0xff);
    PC += 1;
}

void CPU::addC()
{
    ushort result = (ushort)A + (ushort)C;
    flags.Z = ((result & 0xff) == 0);
    flags.S = ((result & 0x80) == 0x80);
    flags.P = evenParity(result);
    flags.C = (result > 0xff);

    A = (unsigned char)(result & 0xff);
    PC += 1;
}

void CPU::addD()
{
    ushort result = (ushort)A + (ushort)D;
    flags.Z = ((result & 0xff) == 0);
    flags.S = ((result & 0x80) == 0x80);
    flags.P = evenParity(result);
    flags.C = (result > 0xff);

    A = (unsigned char)(result & 0xff);
    PC += 1;
}

void CPU::addE()
{
    ushort result = (ushort)A + (ushort)E;
    flags.Z = ((result & 0xff) == 0);
    flags.S = ((result & 0x80) == 0x80);
    flags.P = evenParity(result);
    flags.C = (result > 0xff);

    A = (unsigned char)(result & 0xff);
    PC += 1;
}

void CPU::addH()
{
    ushort result = (ushort)A + (ushort)H;
    flags.Z = ((result & 0xff) == 0);
    flags.S = ((result & 0x80) == 0x80);
    flags.P = evenParity(result);
    flags.C = (result > 0xff);

    A = (unsigned char)(result & 0xff);
    PC += 1;
}

void CPU::addL()
{
    ushort result = (ushort)A + (ushort)L;
    flags.Z = ((result & 0xff) == 0);
    flags.S = ((result & 0x80) == 0x80);
    flags.P = evenParity(result);
    flags.C = (result > 0xff);

    A = (unsigned char)(result & 0xff);
    PC += 1;
}

void CPU::addM()
{
    ushort HLAddr = (ushort)(H << 8) | L;
    ushort result = A + memory[HLAddr];
    flags.Z = ((result & 0xff) == 0);
    flags.S = ((result & 0x80) == 0x80);
    flags.P = evenParity(result);
    flags.C = (result > 0xff);

    A = (unsigned char)(result & 0xff);
    PC += 1;
}

void CPU::addA()
{
    ushort result = (ushort)A + (ushort)A;
    flags.Z = ((result & 0xff) == 0);
    flags.S = ((result & 0x80) == 0x80);
    flags.P = evenParity(result);
    flags.C = (result > 0xff);

    A = (unsigned char)(result & 0xff);
    PC += 1;
}

void CPU::adi()
{
    unsigned char addByte = memory[PC + 1];
    ushort result = (ushort)A + (ushort)addByte;
    flags.Z = ((result & 0xff) == 0);
    flags.S = ((result & 0x80) == 0x80);
    flags.P = evenParity(result);
    flags.C = (result > 0xff);
    
    A = (unsigned char)(result & 0xff);
    PC += 2;
}

void CPU::ani()
{
    unsigned char addByte = memory[PC + 1];
    ushort result = (ushort)A & (ushort)addByte;
    flags.Z = ((result & 0xff) == 0);
    flags.S = ((result & 0x80) == 0x80);
    flags.P = evenParity(result);
    flags.C = 0;
    
    A = (unsigned char)(result & 0xff);
    PC += 2;
}

void CPU::ori()
{
    unsigned char addByte = memory[PC + 1];
    ushort result = (ushort)A | (ushort)addByte;
    flags.Z = ((result & 0xff) == 0);
    flags.S = ((result & 0x80) == 0x80);
    flags.P = evenParity(result);
    flags.C = 0;
    
    A = (unsigned char)(result & 0xff);
    PC += 2;
}

void CPU::xri()
{
    unsigned char addByte = memory[PC + 1];
    ushort result = (ushort)A ^ (ushort)addByte;
    flags.Z = ((result & 0xff) == 0);
    flags.S = ((result & 0x80) == 0x80);
    flags.P = evenParity(result);
    flags.C = 0;
    
    A = (unsigned char)(result & 0xff);
    PC += 2;
}

void CPU::sui()
{
    unsigned char subByte = memory[PC + 1];
    ushort result = (ushort)A - (ushort)subByte;
    flags.Z = ((result & 0xff) == 0);
    flags.S = ((result & 0x80) == 0x80);
    flags.P = evenParity(result);
    flags.C = (result > 0xff);
    
    A = (unsigned char)(result & 0xff);
    PC += 2;
}

void CPU::sbi()
{
    unsigned char subByte = memory[PC + 1];
    ushort result = (ushort)A - (ushort)subByte - (ushort)flags.C;
    flags.Z = ((result & 0xff) == 0);
    flags.S = ((result & 0x80) == 0x80);
    flags.P = evenParity(result);
    flags.C = (result > 0xff);
    
    A = (unsigned char)(result & 0xff);
    PC += 2;
}

void CPU::aci()
{
    unsigned char addByte = memory[PC + 1];
    ushort result = (ushort)A + (ushort)addByte + (ushort)flags.C;
    flags.Z = ((result & 0xff) == 0);
    flags.S = ((result & 0x80) == 0x80);
    flags.P = evenParity(result);
    flags.C = (result > 0xff);
    
    A = (unsigned char)(result & 0xff);
    PC += 2;
}

void CPU::adcB()
{
    ushort result = (ushort)A + (ushort)B + flags.C;
    flags.Z = ((result & 0xff) == 0);
    flags.S = ((result & 0x80) == 0x80);
    flags.P = evenParity(result);
    flags.C = (result > 0xff);

    A = (unsigned char)(result & 0xff);
    PC += 1;
}

void CPU::adcC()
{
    ushort result = (ushort)A + (ushort)C + flags.C;
    flags.Z = ((result & 0xff) == 0);
    flags.S = ((result & 0x80) == 0x80);
    flags.P = evenParity(result);
    flags.C = (result > 0xff);

    A = (unsigned char)(result & 0xff);
    PC += 1;
}

void CPU::adcD()
{
    ushort result = (ushort)A + (ushort)D + flags.C;
    flags.Z = ((result & 0xff) == 0);
    flags.S = ((result & 0x80) == 0x80);
    flags.P = evenParity(result);
    flags.C = (result > 0xff);

    A = (unsigned char)(result & 0xff);
    PC += 1;
}

void CPU::adcE()
{
    ushort result = (ushort)A + (ushort)E + flags.C;
    flags.Z = ((result & 0xff) == 0);
    flags.S = ((result & 0x80) == 0x80);
    flags.P = evenParity(result);
    flags.C = (result > 0xff);

    A = (unsigned char)(result & 0xff);
    PC += 1;
}

void CPU::adcH()
{
    ushort result = (ushort)A + (ushort)H + flags.C;
    flags.Z = ((result & 0xff) == 0);
    flags.S = ((result & 0x80) == 0x80);
    flags.P = evenParity(result);
    flags.C = (result > 0xff);

    A = (unsigned char)(result & 0xff);
    PC += 1;
}

void CPU::adcL()
{
    ushort result = (ushort)A + (ushort)L + flags.C;
    flags.Z = ((result & 0xff) == 0);
    flags.S = ((result & 0x80) == 0x80);
    flags.P = evenParity(result);
    flags.C = (result > 0xff);

    A = (unsigned char)(result & 0xff);
    PC += 1;
}

void CPU::adcM()
{
    ushort HLAddr = (ushort)(H << 8) | L;
    ushort result = A + memory[HLAddr] + flags.C;
    flags.Z = ((result & 0xff) == 0);
    flags.S = ((result & 0x80) == 0x80);
    flags.P = evenParity(result);
    flags.C = (result > 0xff);

    A = (unsigned char)(result & 0xff);
    PC += 1;
}

void CPU::adcA()
{
    ushort result = (ushort)A + (ushort)A + flags.C;
    flags.Z = ((result & 0xff) == 0);
    flags.S = ((result & 0x80) == 0x80);
    flags.P = evenParity(result);
    flags.C = (result > 0xff);

    A = (unsigned char)(result & 0xff);
    PC += 1;
}

void CPU::subB()
{
    ushort result = (ushort)A - (ushort)B;
    flags.Z = ((result & 0xff) == 0);
    flags.S = ((result & 0x80) == 0x80);
    flags.P = evenParity(result);
    flags.C = (result > 0xff);

    A = (unsigned char)(result & 0xff);
    PC += 1;
}

void CPU::subC()
{
    ushort result = (ushort)A - (ushort)C;
    flags.Z = ((result & 0xff) == 0);
    flags.S = ((result & 0x80) == 0x80);
    flags.P = evenParity(result);
    flags.C = (result > 0xff);

    A = (unsigned char)(result & 0xff);
    PC += 1;
}

void CPU::subD()
{
    ushort result = (ushort)A - (ushort)D;
    flags.Z = ((result & 0xff) == 0);
    flags.S = ((result & 0x80) == 0x80);
    flags.P = evenParity(result);
    flags.C = (result > 0xff);

    A = (unsigned char)(result & 0xff);
    PC += 1;
}

void CPU::subE()
{
    ushort result = (ushort)A - (ushort)E;
    flags.Z = ((result & 0xff) == 0);
    flags.S = ((result & 0x80) == 0x80);
    flags.P = evenParity(result);
    flags.C = (result > 0xff);

    A = (unsigned char)(result & 0xff);
    PC += 1;
}

void CPU::subH()
{
    ushort result = (ushort)A - (ushort)H;
    flags.Z = ((result & 0xff) == 0);
    flags.S = ((result & 0x80) == 0x80);
    flags.P = evenParity(result);
    flags.C = (result > 0xff);

    A = (unsigned char)(result & 0xff);
    PC += 1;
}

void CPU::subL()
{
    ushort result = (ushort)A - (ushort)L;
    flags.Z = ((result & 0xff) == 0);
    flags.S = ((result & 0x80) == 0x80);
    flags.P = evenParity(result);
    flags.C = (result > 0xff);

    A = (unsigned char)(result & 0xff);
    PC += 1;
}

void CPU::subM()
{
    ushort HLAddr = (ushort)(H << 8) | L;
    ushort result = A - memory[HLAddr];
    flags.Z = ((result & 0xff) == 0);
    flags.S = ((result & 0x80) == 0x80);
    flags.P = evenParity(result);
    flags.C = (result > 0xff);

    A = (unsigned char)(result & 0xff);
    PC += 1;
}

void CPU::subA()
{
    ushort result = (ushort)A - (ushort)A;
    flags.Z = ((result & 0xff) == 0);
    flags.S = ((result & 0x80) == 0x80);
    flags.P = evenParity(result);
    flags.C = (result > 0xff);

    A = (unsigned char)(result & 0xff);
    PC += 1;
}

void CPU::sbbB()
{
    ushort result = (ushort)A - (ushort)B - flags.C;
    flags.Z = ((result & 0xff) == 0);
    flags.S = ((result & 0x80) == 0x80);
    flags.P = evenParity(result);
    flags.C = (result > 0xff);

    A = (unsigned char)(result & 0xff);
    PC += 1;
}

void CPU::sbbC()
{
    ushort result = (ushort)A - (ushort)C - flags.C;
    flags.Z = ((result & 0xff) == 0);
    flags.S = ((result & 0x80) == 0x80);
    flags.P = evenParity(result);
    flags.C = (result > 0xff);

    A = (unsigned char)(result & 0xff);
    PC += 1;
}

void CPU::sbbD()
{
    ushort result = (ushort)A - (ushort)D - flags.C;
    flags.Z = ((result & 0xff) == 0);
    flags.S = ((result & 0x80) == 0x80);
    flags.P = evenParity(result);
    flags.C = (result > 0xff);

    A = (unsigned char)(result & 0xff);
    PC += 1;
}

void CPU::sbbE()
{
    ushort result = (ushort)A - (ushort)E - flags.C;
    flags.Z = ((result & 0xff) == 0);
    flags.S = ((result & 0x80) == 0x80);
    flags.P = evenParity(result);
    flags.C = (result > 0xff);

    A = (unsigned char)(result & 0xff);
    PC += 1;
}

void CPU::sbbH()
{
    ushort result = (ushort)A - (ushort)H - flags.C;
    flags.Z = ((result & 0xff) == 0);
    flags.S = ((result & 0x80) == 0x80);
    flags.P = evenParity(result);
    flags.C = (result > 0xff);

    A = (unsigned char)(result & 0xff);
    PC += 1;
}

void CPU::sbbL()
{
    ushort result = (ushort)A - (ushort)L - flags.C;
    flags.Z = ((result & 0xff) == 0);
    flags.S = ((result & 0x80) == 0x80);
    flags.P = evenParity(result);
    flags.C = (result > 0xff);

    A = (unsigned char)(result & 0xff);
    PC += 1;
}

void CPU::sbbM()
{
    ushort HLAddr = (ushort)(H << 8) | L;
    ushort result = A - memory[HLAddr] - flags.C;
    flags.Z = ((result & 0xff) == 0);
    flags.S = ((result & 0x80) == 0x80);
    flags.P = evenParity(result);
    flags.C = (result > 0xff);

    A = (unsigned char)(result & 0xff);
    PC += 1;
}

void CPU::sbbA()
{
    ushort result = (ushort)A - (ushort)A - flags.C;
    flags.Z = ((result & 0xff) == 0);
    flags.S = ((result & 0x80) == 0x80);
    flags.P = evenParity(result);
    flags.C = (result > 0xff);

    A = (unsigned char)(result & 0xff);
    PC += 1;
}

void CPU::anaB()
{
    unsigned char result = A & B;
    flags.Z = (result == 0);
    flags.S = ((result & 0x80) == 0x80);
    flags.P = evenParity(result);
    flags.C = 0; //AND clears carry flag

    A = result;
    PC += 1;
}

void CPU::anaC()
{
    unsigned char result = A & C;
    flags.Z = (result == 0);
    flags.S = ((result & 0x80) == 0x80);
    flags.P = evenParity(result);
    flags.C = 0; 

    A = result;
    PC += 1;
}

void CPU::anaD()
{
    unsigned char result = A & D;
    flags.Z = (result == 0);
    flags.S = ((result & 0x80) == 0x80);
    flags.P = evenParity(result);
    flags.C = 0; 

    A = result;
    PC += 1;
}

void CPU::anaE()
{
    unsigned char result = A & E;
    flags.Z = (result == 0);
    flags.S = ((result & 0x80) == 0x80);
    flags.P = evenParity(result);
    flags.C = 0; 

    A = result;
    PC += 1;
}

void CPU::anaH()
{
    unsigned char result = A & H;
    flags.Z = (result == 0);
    flags.S = ((result & 0x80) == 0x80);
    flags.P = evenParity(result);
    flags.C = 0; 

    A = result;
    PC += 1;
}

void CPU::anaL()
{
    unsigned char result = A & L;
    flags.Z = (result == 0);
    flags.S = ((result & 0x80) == 0x80);
    flags.P = evenParity(result);
    flags.C = 0; 

    A = result;
    PC += 1;
}

void CPU::anaM()
{
    ushort HLAddr = (ushort)(H << 8) | L;
    unsigned char result = A & memory[HLAddr];
    flags.Z = (result == 0);
    flags.S = ((result & 0x80) == 0x80);
    flags.P = evenParity(result);
    flags.C = 0; 

    A = result;
    PC += 1;
}

void CPU::anaA()
{
    unsigned char result = A & A;
    flags.Z = (result == 0);
    flags.S = ((result & 0x80) == 0x80);
    flags.P = evenParity(result);
    flags.C = 0; 

    A = result;
    PC += 1;
}

void CPU::xraB()
{
    unsigned char result = A ^ B;
    flags.Z = (result == 0);
    flags.S = ((result & 0x80) == 0x80);
    flags.P = evenParity(result);
    flags.C = 0; 

    A = result;
    PC += 1;
}

void CPU::xraC()
{
    unsigned char result = A ^ C;
    flags.Z = (result == 0);
    flags.S = ((result & 0x80) == 0x80);
    flags.P = evenParity(result);
    flags.C = 0; 

    A = result;
    PC += 1;
}

void CPU::xraD()
{
    unsigned char result = A ^ D;
    flags.Z = (result == 0);
    flags.S = ((result & 0x80) == 0x80);
    flags.P = evenParity(result);
    flags.C = 0; 

    A = result;
    PC += 1;
}

void CPU::xraE()
{
    unsigned char result = A ^ E;
    flags.Z = (result == 0);
    flags.S = ((result & 0x80) == 0x80);
    flags.P = evenParity(result);
    flags.C = 0; 

    A = result;
    PC += 1;
}

void CPU::xraH()
{
    unsigned char result = A ^ H;
    flags.Z = (result == 0);
    flags.S = ((result & 0x80) == 0x80);
    flags.P = evenParity(result);
    flags.C = 0; 

    A = result;
    PC += 1;
}

void CPU::xraL()
{
    unsigned char result = A ^ L;
    flags.Z = (result == 0);
    flags.S = ((result & 0x80) == 0x80);
    flags.P = evenParity(result);
    flags.C = 0; 

    A = result;
    PC += 1;
}

void CPU::xraM()
{
    ushort HLAddr = (ushort)(H << 8) | L;
    unsigned char result = A ^ memory[HLAddr];
    flags.Z = (result == 0);
    flags.S = ((result & 0x80) == 0x80);
    flags.P = evenParity(result);
    flags.C = 0; 

    A = result;
    PC += 1;
}

void CPU::xraA()
{
    unsigned char result = A ^ A;
    flags.Z = (result == 0);
    flags.S = ((result & 0x80) == 0x80);
    flags.P = evenParity(result);
    flags.C = 0; 

    A = result;
    PC += 1;
}

void CPU::oraB()
{
    unsigned char result = A | B;
    flags.Z = (result == 0);
    flags.S = ((result & 0x80) == 0x80);
    flags.P = evenParity(result);
    flags.C = 0; 

    A = result;
    PC += 1;
}

void CPU::oraC()
{
    unsigned char result = A | C;
    flags.Z = (result == 0);
    flags.S = ((result & 0x80) == 0x80);
    flags.P = evenParity(result);
    flags.C = 0; 

    A = result;
    PC += 1;
}

void CPU::oraD()
{
    unsigned char result = A | D;
    flags.Z = (result == 0);
    flags.S = ((result & 0x80) == 0x80);
    flags.P = evenParity(result);
    flags.C = 0; 

    A = result;
    PC += 1;
}

void CPU::oraE()
{
    unsigned char result = A | E;
    flags.Z = (result == 0);
    flags.S = ((result & 0x80) == 0x80);
    flags.P = evenParity(result);
    flags.C = 0; 

    A = result;
    PC += 1;
}

void CPU::oraH()
{
    unsigned char result = A | H;
    flags.Z = (result == 0);
    flags.S = ((result & 0x80) == 0x80);
    flags.P = evenParity(result);
    flags.C = 0; 

    A = result;
    PC += 1;
}

void CPU::oraL()
{
    unsigned char result = A | L;
    flags.Z = (result == 0);
    flags.S = ((result & 0x80) == 0x80);
    flags.P = evenParity(result);
    flags.C = 0; 

    A = result;
    PC += 1;
}

void CPU::oraM()
{
    ushort HLAddr = (ushort)(H << 8) | L;
    unsigned char result = A | memory[HLAddr];
    flags.Z = (result == 0);
    flags.S = ((result & 0x80) == 0x80);
    flags.P = evenParity(result);
    flags.C = 0; 

    A = result;
    PC += 1;
}

void CPU::oraA()
{
    unsigned char result = A | A;
    flags.Z = (result == 0);
    flags.S = ((result & 0x80) == 0x80);
    flags.P = evenParity(result);
    flags.C = 0; 

    A = result;
    PC += 1;
}

void CPU::cmpB()
{
    ushort result = (ushort)A - (ushort)B;
    flags.Z = ((result & 0xff) == 0);
    flags.S = ((result & 0x80) == 0x80);
    flags.P = evenParity(result);
    flags.C = (result > 0xff); 

    PC += 1;
}

void CPU::cmpC()
{
    ushort result = (ushort)A - (ushort)C;
    flags.Z = ((result & 0xff) == 0);
    flags.S = ((result & 0x80) == 0x80);
    flags.P = evenParity(result);
    flags.C = (result > 0xff); 

    PC += 1;
}

void CPU::cmpD()
{
    ushort result = (ushort)A - (ushort)D;
    flags.Z = ((result & 0xff) == 0);
    flags.S = ((result & 0x80) == 0x80);
    flags.P = evenParity(result);
    flags.C = (result > 0xff); 

    PC += 1;
}

void CPU::cmpE()
{
    ushort result = (ushort)A - (ushort)E;
    flags.Z = ((result & 0xff) == 0);
    flags.S = ((result & 0x80) == 0x80);
    flags.P = evenParity(result);
    flags.C = (result > 0xff); 

    PC += 1;
}

void CPU::cmpH()
{
    ushort result = (ushort)A - (ushort)H;
    flags.Z = ((result & 0xff) == 0);
    flags.S = ((result & 0x80) == 0x80);
    flags.P = evenParity(result);
    flags.C = (result > 0xff); 

    PC += 1;
}

void CPU::cmpL()
{
    ushort result = (ushort)A - (ushort)L;
    flags.Z = ((result & 0xff) == 0);
    flags.S = ((result & 0x80) == 0x80);
    flags.P = evenParity(result);
    flags.C = (result > 0xff); 

    PC += 1;
}

void CPU::cmpM()
{
    ushort HLAddr = (ushort)(H << 8) | L;
    ushort result = (ushort)A - memory[HLAddr];
    flags.Z = ((result & 0xff) == 0);
    flags.S = ((result & 0x80) == 0x80);
    flags.P = evenParity(result);
    flags.C = (result > 0xff); 

    PC += 1;
}

void CPU::cmpA()
{
    ushort result = (ushort)A - (ushort)A;
    flags.Z = ((result & 0xff) == 0);
    flags.S = ((result & 0x80) == 0x80);
    flags.P = evenParity(result);
    flags.C = (result > 0xff); 

    PC += 1;
}

void CPU::rnz()
{
    if (!flags.Z)
    {
        ret();
    }
    else
    {
        PC += 1;
    }
}

void CPU::rz()
{
    if (flags.Z)
    {
        ret();
    }
    else
    {
        PC += 1;
    }
}

void CPU::popB()
{
    C = memory[SP];
    B = memory[SP + 1];
    SP += 2;
    PC += 1;
}

void CPU::popD()
{
    E = memory[SP];
    D = memory[SP + 1];
    SP += 2;
    PC += 1;
}

void CPU::popH()
{
    L = memory[SP];
    H = memory[SP + 1];
    SP += 2;
    PC += 1;
}

void CPU::popPSW()
{
    A = memory[SP + 1];
    unsigned char PSW = memory[SP];

    //set flags according to PSW byte
    flags.Z = ((PSW & 0x01) == 0x01);
    flags.S = ((PSW & 0x02) == 0x02);
    flags.P = ((PSW & 0x04) == 0x04);
    flags.C = ((PSW & 0x08) == 0x08);

    SP += 2;
    PC += 1;
}

void CPU::pushPSW()
{
    writeMem(SP - 1, A);
    unsigned char PSW  = (flags.Z | 
                          flags.S << 1 |
                          flags.P << 2 |
                          flags.C << 3 |
                          0);
    writeMem(SP - 2, PSW);

    SP -= 2;
    PC += 1;
}

void CPU::pushB()
{
    writeMem(SP - 2, C);
    writeMem(SP - 1, B);
    SP -= 2;
    PC += 1;
}

void CPU::pushD()
{
    writeMem(SP - 2, E);
    writeMem(SP - 1, D);
    SP -= 2;
    PC += 1;
}

void CPU::pushH()
{
    writeMem(SP - 2, L);
    writeMem(SP - 1, H);
    SP -= 2;
    PC += 1;
}

void CPU::jnz()
{
    if (!flags.Z)
    {
        PC = (memory[PC + 2] << 8) | memory[PC + 1];
    }
    else
    {
        PC += 3;
    }
}

void CPU::jnc()
{
    if (!flags.C)
    {
        PC = (memory[PC + 2] << 8) | memory[PC + 1];
    }
    else
    {
        PC += 3;
    }
}

void CPU::jpo()
{
    if (!flags.P)
    {
        PC = (memory[PC + 2] << 8) | memory[PC + 1];
    }
    else
    {
        PC += 3;
    }
}

void CPU::jp()
{
    if (!flags.S)
    {
        PC = (memory[PC + 2] << 8) | memory[PC + 1];
    }
    else
    {
        PC += 3;
    }
}

void CPU::jpe()
{
    if (flags.P)
    {
        PC = (memory[PC + 2] << 8) | memory[PC + 1];
    }
    else
    {
        PC += 3;
    }
}

void CPU::jc()
{
    if (flags.C)
    {
        PC = (memory[PC + 2] << 8) | memory[PC + 1];
    }
    else
    {
        PC += 3;
    }
}

void CPU::jz()
{
    if (flags.Z)
    {
        PC = (memory[PC + 2] << 8) | memory[PC + 1];
    }
    else
    {
        PC += 3;
    }
}

void CPU::jm()
{
    if (flags.S)
    {
        PC = (memory[PC + 2] << 8) | memory[PC + 1];
    }
    else
    {
        PC += 3;
    }
}

void CPU::jmp()
{
    PC = (memory[PC + 2] << 8) | memory[PC + 1];
}


void CPU::cnz()
{
    if (!flags.Z)
    {
        call();
    }
    else
    {
        PC += 3;
    }
}

void CPU::cnc()
{
    if (!flags.C)
    {
        call();
    }
    else
    {
        PC += 3;
    }
}

void CPU::cc()
{
    if (flags.C)
    {
        call();
    }
    else
    {
        PC += 3;
    }
}

void CPU::cz()
{
    if (flags.Z)
    {
        call();
    }
    else
    {
        PC += 3;
    }
}

void CPU::call()
{
    //store the start address of the next instruction on stack
    ushort returnAddr = PC + 3;
    writeMem(SP - 1, (unsigned char)((returnAddr >> 8) & 0xff));
    writeMem(SP - 2, (unsigned char)(returnAddr & 0x00ff));
    SP -= 2;
    PC = (memory[PC + 2] << 8) | memory[PC + 1];
}

void CPU::ret()
{
    PC = (ushort)(memory[SP + 1] << 8) | memory[SP];
    SP += 2;
    //PC += 1;
}

void CPU::rst0()
{
    writeMem(SP - 1, (unsigned char)((PC >> 8) & 0xff));
    writeMem(SP - 2, (unsigned char)(PC & 0x00ff));
    SP -= 2;
    PC = 0x0;
}

void CPU::rst1()
{
    writeMem(SP - 1, (unsigned char)((PC >> 8) & 0xff));
    writeMem(SP - 2, (unsigned char)(PC & 0x00ff));
    SP -= 2;
    PC = 0x8;
}

void CPU::rst2()
{
    writeMem(SP - 1, (unsigned char)((PC >> 8) & 0xff));
    writeMem(SP - 2, (unsigned char)(PC & 0x00ff));
    SP -= 2;
    PC = 0x10;
}

void CPU::rst3()
{
    writeMem(SP - 1, (unsigned char)((PC >> 8) & 0xff));
    writeMem(SP - 2, (unsigned char)(PC & 0x00ff));
    SP -= 2;
    PC = 0x18;
}

void CPU::rst4()
{
    writeMem(SP - 1, (unsigned char)((PC >> 8) & 0xff));
    writeMem(SP - 2, (unsigned char)(PC & 0x00ff));
    SP -= 2;
    PC = 0x20;
}

void CPU::rst5()
{
    writeMem(SP - 1, (unsigned char)((PC >> 8) & 0xff));
    writeMem(SP - 2, (unsigned char)(PC & 0x00ff));
    SP -= 2;
    PC = 0x28;
}

void CPU::rst6()
{
    writeMem(SP - 1, (unsigned char)((PC >> 8) & 0xff));
    writeMem(SP - 2, (unsigned char)(PC & 0x00ff));
    SP -= 2;
    PC = 0x30;
}

void CPU::rst7()
{
    writeMem(SP - 1, (unsigned char)((PC >> 8) & 0xff));
    writeMem(SP - 2, (unsigned char)(PC & 0x00ff));
    SP -= 2;
    PC = 0x38;
}

void CPU::rnc()
{
    if (!flags.C)
    {
        ret();
    }
    else
    {
        PC += 1;
    }
}

void CPU::rc()
{
    if (flags.C)
    {
        ret();
    }
    else
    {
        PC += 1;
    }
}

void CPU::rpo()
{
    if (!flags.P)
    {
        ret();
    }
    else
    {
        PC += 1;
    }
}

void CPU::rp()
{
    if (!flags.S)
    {
        ret();
    }
    else
    {
        PC += 1;
    }
}

void CPU::rpe()
{
    if (flags.P)
    {
        ret();
    }
    else
    {
        PC += 1;
    }
}

void CPU::cpo()
{
    if (!flags.P)
    {
        call();
    }
    else
    {
        PC += 3;
    }
}

void CPU::cpe()
{
    if (flags.P)
    {
        call();
    }
    else
    {
        PC += 3;
    }
}

void CPU::cp()
{
    if (!flags.S)
    {
        call();
    }
    else
    {
        PC += 3;
    }
}

void CPU::cpi()
{
    ushort result = (ushort)A - (ushort)memory[PC + 1];
    flags.Z = ((result & 0xff) == 0);
    flags.S = ((result & 0x80) == 0x80);
    flags.P = evenParity(result);
    flags.C = ((ushort)A < (ushort)memory[PC + 1]); 

    PC += 2;
}

void CPU::xthl()
{
    unsigned char temp = memory[SP];
    writeMem(SP, L);
    L = temp;

    temp = memory[SP + 1];
    writeMem(SP + 1, H);
    H = temp;

    PC += 1;
}

void CPU::pchl()
{
    PC = (ushort)(H << 8) | L;
}

void CPU::xchg()
{
    //swap D and H
    unsigned char temp = H;
    H = D;
    D = temp;

    //swap L and E
    temp = L;
    L = E;
    E = temp;

    PC += 1;
}

void CPU::sphl()
{
    SP = (ushort)(H << 8) | L;
    PC += 1;
}

void CPU::ei()
{
    interruptsEnabled = true;
    PC += 1;
}

void CPU::di()
{
    interruptsEnabled = false;
    PC += 1;
}

void CPU::daa()
{
    unsigned char AHigh = A >> 4;
    unsigned char ALow = A & 0x0f;
    unsigned char addValue = 0;
    bool carry = false;

    if (flags.AC || ALow > 0x9)
    {
        addValue = 0x6;
    }
    if (flags.C || AHigh > 0x9 || (AHigh >= 9 && ALow > 9))
    {
        addValue = 0x60;
        carry = true;
    }

    ushort result = (ushort)A + (ushort)addValue;
    flags.Z = (result & 0xff == 0);
    flags.S = ((result & 0x80) == 0x80);
    flags.P = evenParity(A);
    flags.C = carry;

    A = result;
    PC += 1;
}

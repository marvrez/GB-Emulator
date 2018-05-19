#include "apu.h"

//register addresses
const Address SM1_SWEEP = 0xFF10; //Sound mode 1 register
const Address SM1_LENGTH = 0xFF11;
const Address SM1_ENVELOPE = 0xFF12;
const Address SM1_FREQ_LO = 0xFF13;
const Address SM1_FREQ_HI = 0xFF14;
const Address SM2_LENGTH = 0xFF16;
const Address SM2_ENVELOPE = 0xFF17;
const Address SM2_FREQ_LO = 0xFF18;
const Address SM2_FREQ_HI = 0xFF19;
const Address SM3_CONTROL = 0xFF1A;
const Address SM3_LENGTH = 0xFF1B;
const Address SM3_SELECT = 0xFF1C;
const Address SM3_FREQ_LO = 0xFF1D;
const Address SM3_FREQ_HI = 0xFF1E;
const Address SM4_LENGTH = 0xFF20;
const Address SM4_ENVELOPE = 0xFF21;
const Address SM4_POLY = 0xFF22;
const Address SM4_COUNTER = 0xFF23;
const Address S_CHANNEL_CONTROL = 0xFF24; //channel control
const Address S_SELECT = 0xFF25; //selection of sound output terminal
const Address S_CONTROL = 0xFF26; //Sound on/off
const Address S_WAVE_RAM = 0xFF30; //wave pattern RAM

APU::APU(std::shared_ptr<MMU> &mmu) :
    mmu(mmu)
{
    mmu->writeByte(S_CONTROL, 0x00); //set sound off

    ch1Sound.setBuffer(ch1Buffer);
    ch1Sound.setLoop(false);

    ch2Sound.setBuffer(ch2Buffer);
    ch2Sound.setLoop(false);

    ch3Sound.setBuffer(ch3Buffer);
    ch3Sound.setLoop(false);

    ch4Sound.setBuffer(ch4Buffer);
    ch4Sound.setLoop(false);
}

void APU::tick(Cycles cycle) {
    int freq;
    int duty;
    int vol;

    //Turn On sound
    u8 s_control = mmu->readByte(S_CONTROL);
    if ((s_control & 0x80) && !soundOn) soundOn = true;

    //Turn Off Sound if already playing
    else if (!(s_control & 0x80) && soundOn) {
        soundOn = false;
        ch1On = false;
        ch2On = false;
        ch3On = false;

        framesyncCycle = 0;

        //reset all sound registers
        for (u16 address = 0xFF10; address < 0xFF30; ++address)
            mmu->writeByte(address, 0x00);
    }

    if (!soundOn) {
        for (u16 address = 0xFF10; address < 0xFF27; ++address)
            mmu->writeByte(address, 0x000);
    }

    //Record value on buffer
    if (soundOn) {
        //Channel 1 - Square
        ch1Count -= cycle.cycles;
        while (ch1Count <= 0) {
            freq = ((mmu->readByte(SM1_FREQ_HI) & 7) << 8) | mmu->readByte(SM1_FREQ_LO);

            ch1Count += (2048 - freq) << 2;
            ch1DutyIndex = (ch1DutyIndex + 1) & 7;
        }

        //Channel 2 - Square
        ch2Count -= cycle.cycles;
        while (ch2Count <= 0) {
            freq = ((mmu->readByte(SM2_FREQ_HI) & 7) << 8) | mmu->readByte(SM2_FREQ_LO);

            ch2Count += (2048 - freq) << 2;
            ch2DutyIndex = (ch2DutyIndex + 1) & 7;
        }

        //Channel 3 - Wave
        ch3Count -= cycle.cycles;
        while (ch3Count <= 0) {
            freq = ((mmu->readByte(SM3_FREQ_HI) & 7) << 8) | mmu->readByte(SM3_FREQ_LO);

            ch3Count += (2048 - freq) << 2;
            ch3Position = (ch3Position + 1) & 0x1F;
        }

        //Channel 4 - Noise
        ch4Count -= cycle.cycles;
        while (ch4Count <= 0) {
            freq = mmu->readByte(SM4_POLY);
            freq = Ch4Div[freq&0x7] << ((freq>>4) & 0xF);

            ch4Count += freq;
            duty = (((ch4Lsfr>>1) ^ ch4Lsfr) & 1);
            ch4Lsfr = (ch4Lsfr>>1) | (duty<<14);
            if (freq & 8) ch4Lsfr = (duty<<6) | (ch4Lsfr&0xFB);
        }

        sampleCount -= cycle.cycles;
        while (sampleCount <= 0) {
            u8 soundControl = mmu->readByte(S_CONTROL);
            sampleCount += cyclesPerSample;

            //if sound is on
            if (soundControl & 0x80) {
                //Channel 1 - Square
                if (ch1On) {
                    duty = (mmu->readByte(SM1_LENGTH) >> 6) & 3;
                    vol = (mmu->readByte(SM1_ENVELOPE) >> 4);
                    ch1Raw[sampleIndex] = vol * Duty[duty][ch1DutyIndex] * AMPLITUDE;
                }
                else ch1Raw[sampleIndex] = 0;

                //Channel 2 - Square
                if (ch2On) {
                    duty = (mmu->readByte(SM2_LENGTH) >> 6) & 3;
                    vol = (mmu->readByte(SM2_ENVELOPE) >> 4);
                    ch2Raw[sampleIndex] = vol * Duty[duty][ch2DutyIndex] * AMPLITUDE;
                }
                else ch2Raw[sampleIndex] = 0;

                //Channel 3 - Wave
                if (ch3On) {
                    duty = mmu->readByte(S_WAVE_RAM + (ch3Position >> 1));
                    vol = ((mmu->readByte(SM3_SELECT) >> 5) - 1) & 0x3;

                    duty = ((duty >> (ch3Position & 1)) & 0xF) >> vol;

                    ch3Raw[sampleIndex] = duty;
                }
                else ch3Raw[sampleIndex] = 0;

                //Channel 4 - Noise
                if (ch4On) {
                    vol = ((mmu->readByte(SM4_ENVELOPE) >> 4) & 0xF);
                    ch4Raw[sampleIndex] = (ch4Lsfr & 1) * vol;
                }
                else ch4Raw[sampleIndex] = 0;
            }

            sampleIndex += 1;
            //play sound from buffer
            if (sampleIndex >= SAMPLES) {
                sampleIndex = 0;

                ch1Buffer.loadFromSamples(ch1Raw, SAMPLES, 1, SAMPLE_RATE);
                ch1Sound.play();

                ch2Buffer.loadFromSamples(ch2Raw, SAMPLES, 1, SAMPLE_RATE);
                ch2Sound.play();

                ch3Buffer.loadFromSamples(ch3Raw, SAMPLES, 1, SAMPLE_RATE);
                ch3Sound.play();

                ch4Buffer.loadFromSamples(ch4Raw, SAMPLES, 1, SAMPLE_RATE);
                //ch4Sound.play();
            }
        }
    }

    //FrameSync
    framesyncCycle += cycle.cycles;
    if (framesyncCycle > FramesyncPeriodCount) {
        checkTrigger();
        framesyncCount -= FramesyncPeriodCount;
        switch (framesyncCount) {
        case 0:
            advanceLength();
            break;
        case 2:
            advanceLength();
            advanceSweep();
            break;
        case 4:
            advanceLength();
            break;
        case 6:
            advanceLength();
            advanceSweep();
            break;
        case 7:
            advanceEnvelope();
            break;
        default:
            break;
        }
        framesyncCount = (framesyncCount + 1) & 7;
    }
}

void APU::checkTrigger() {
    u8 sweep, length, envelope;
    u8 lowFreq, highFreq;

    //Channel 1 - Square
    highFreq = mmu->readByte(SM1_FREQ_HI);
    if (highFreq & 0x80) {
        sweep		= mmu->readByte(SM1_SWEEP);
        length		= mmu->readByte(SM1_LENGTH) & 0x3F;
        envelope	= mmu->readByte(SM1_ENVELOPE);
        lowFreq		= mmu->readByte(SM1_FREQ_LO);

        ch1On				= true;
        ch1DutyIndex		= 0;
        ch1SweepCount		= (sweep>>4)&7;
        ch1SweepEn			= (sweep&0x77) != 0;
        ch1SweepShadow		= ((highFreq&7)<<8) | lowFreq;
        ch1EnvolopeEn		= true;
        ch1EnvelopeCount	= envelope&0x7;
        ch1Count			= (2048 - ch1SweepShadow)<<2;

        if ((highFreq & 0x40) == 0 && length == 0)
            mmu->writeByte(SM1_LENGTH, mmu->readByte(SM1_LENGTH) | 0x3F);
        if ((sweep & 7) != 0 && (calculateSweep() > 2047)) {
            ch1On = false;
            mmu->writeByte(S_CONTROL, mmu->readByte(S_CONTROL) & 0xFE);
        }
        else mmu->writeByte(S_CONTROL, mmu->readByte(S_CONTROL) | 0x01);

        mmu->writeByte(SM1_FREQ_HI, highFreq & 0x7F);
    }

    //Channel 2 - Square
    highFreq = mmu->readByte(SM2_FREQ_HI);
    if (highFreq & 0x80) {
        length		= mmu->readByte(SM2_LENGTH) & 0x3F;
        envelope	= mmu->readByte(SM2_ENVELOPE);
        lowFreq		= mmu->readByte(SM2_FREQ_LO);

        ch2On				= true;
        ch2EnvelopeEn		= true;
        ch2EnvelopeCount	= envelope&0x7;
        ch2Count			= (2048 - (((highFreq&7)<<8) | lowFreq))<<2;
        mmu->writeByte(SM2_FREQ_HI, highFreq & 0x7F);
        mmu->writeByte(S_CONTROL, mmu->readByte(S_CONTROL) | 0x02);

        if ((highFreq & 0x40) == 0 && length == 0)
            mmu->writeByte(SM2_LENGTH, mmu->readByte(SM2_LENGTH) | 0x3F);
    }

    //Channel 3 - Wave
    highFreq = mmu->readByte(SM3_FREQ_HI);
    if (highFreq & 0x80) {
        length		= mmu->readByte(SM3_LENGTH);
        lowFreq		= mmu->readByte(SM3_FREQ_LO);

        ch3On			= true;
        ch3Position		= 0;
        ch3Count		= (2048 - (((highFreq&7)<<8) | lowFreq))<<2;
        mmu->writeByte(SM3_FREQ_HI, highFreq & 0x7F);
        mmu->writeByte(S_CONTROL, mmu->readByte(S_CONTROL) | 0x04);
        if ((highFreq & 0x40) == 0 && length == 0)
            mmu->writeByte(SM3_LENGTH, 0xFF);
    }

    //Channel 4 - Noise
    highFreq = mmu->readByte(SM4_COUNTER);
    if (highFreq & 0x80) {
        length		= mmu->readByte(SM4_LENGTH) & 0x3F;
        envelope	= mmu->readByte(SM4_ENVELOPE);
        lowFreq		= mmu->readByte(SM4_POLY);

        ch4On				= true;
        ch4EnvelopeEn		= true;
        ch4EnvelopeCount	= envelope & 0x7;
        ch4Count			= Ch4Div[lowFreq&0x7] << ((lowFreq>>4) & 0xF);
        ch4Lsfr				= 0x7FFF;
        mmu->writeByte(SM4_COUNTER, highFreq & 0x7F);
        mmu->writeByte(S_CONTROL, mmu->readByte(S_CONTROL) | 0x08);

        if ((highFreq & 0x40) == 0 && length == 0)
            mmu->writeByte(SM4_LENGTH, mmu->readByte(SM4_LENGTH) | 0x3F);
    }
}

void APU::advanceSweep() {
    u8 sweepReg;
    int load, sweep;

    //Channel 1
    ch1SweepCount = (ch1SweepCount-1) & 0x7;
    if (ch1SweepCount <= 0) {
        sweepReg	= mmu->readByte(SM1_SWEEP);
        load		= (sweepReg>>4)&7;
        sweep		= (sweepReg&7);

        ch1SweepCount = load;
        if (ch1SweepCount && load) {
            int newFreq = calculateSweep();
            if (newFreq > 2047) {
                ch1On = false;
                mmu->writeByte(S_CONTROL, mmu->readByte(S_CONTROL) & 0xFE);
            }
            else if (sweep > 0) {
                ch1SweepShadow = newFreq;
                mmu->writeByte(SM1_FREQ_LO, ch1SweepShadow & 0xFF);
                mmu->writeByte(SM1_FREQ_HI, ((ch1SweepShadow >> 8) & 0x7) | (mmu->readByte(SM1_FREQ_HI)&0xF8));
            }

            if (calculateSweep() > 2047) {
                ch1On = false;
                mmu->writeByte(S_CONTROL, mmu->readByte(S_CONTROL) & 0xFE);
            }
        }
    }
}

void APU::advanceLength() {
    int length, reload;

    //Channel 1 - Square
    length		= mmu->readByte(SM1_LENGTH)  & 0x3F;
    reload		= mmu->readByte(SM1_FREQ_HI) & 0x40;
    if (reload) {
        ++length;
        if (length >= 64) {
            ch1On = false;
            mmu->writeByte(S_CONTROL, mmu->readByte(S_CONTROL) & 0xFE);
        }
        mmu->writeByte(SM1_LENGTH, (mmu->readByte(SM1_LENGTH) & 0xC0) | (length & 0x3F));
    }

    //Channel 2 - Square
    length		= mmu->readByte(SM2_LENGTH)  & 0x3F;
    reload		= mmu->readByte(SM2_FREQ_HI) & 0x40;
    if (reload) {
        ++length;
        if (length >= 64 ) {
            ch2On = false;
            mmu->writeByte(S_CONTROL, mmu->readByte(S_CONTROL) & 0xFD);
        }
        mmu->writeByte(SM2_LENGTH, (mmu->readByte(SM2_LENGTH) & 0xC0) | (length & 0x3F));
    }

    //Channel 3 - Wave
    length		= mmu->readByte(SM3_LENGTH);
    reload		= mmu->readByte(SM3_FREQ_HI) & 0x40;
    if (reload) {
        ++length;
        if (length >= 256 ) {
            ch3On = false;
            mmu->writeByte(S_CONTROL, mmu->readByte(S_CONTROL) & 0xFB);
        }
        mmu->writeByte(SM3_LENGTH, length & 0xFF);
    }

    //Channel 4 - Noise
    length = mmu->readByte(SM4_LENGTH) & 0x3F;
    reload = mmu->readByte(SM4_COUNTER) & 0x40;
    if (reload) {
        ++length;
        if (length >= 64) {
            ch4On = false;
            mmu->writeByte(S_CONTROL, mmu->readByte(S_CONTROL) & 0xF7);
        }
        mmu->writeByte(SM4_LENGTH, length & 0x3F);
    }
}

void APU::advanceEnvelope() {
    int envelopePeriod, envelopeDir, envelopeVol;

    //Channel 1 - Square
    if (ch1EnvolopeEn) {
        ch1EnvelopeCount = (ch1EnvelopeCount - 1) & 0x7;
        if (ch1EnvelopeCount <= 0) {

            envelopePeriod = mmu->readByte(SM1_ENVELOPE) & 7;
            envelopeDir = mmu->readByte(SM1_ENVELOPE) & 8;
            envelopeVol = mmu->readByte(SM1_ENVELOPE) >> 4;

            ch1EnvelopeCount = envelopePeriod;
            if (envelopePeriod) {
                if (envelopeDir && envelopeVol < 15)
                    mmu->writeByte(SM1_ENVELOPE, mmu->readByte(SM1_ENVELOPE) + 0x10);
                else if (!envelopeDir && envelopeVol > 0)
                    mmu->writeByte(SM1_ENVELOPE, mmu->readByte(SM1_ENVELOPE) - 0x10);
                else ch1EnvolopeEn = false;
            }

        }
    }

    //Channel 2 - Square
    if (ch2EnvelopeEn) {
        ch2EnvelopeCount = (ch2EnvelopeCount - 1) & 0x7;
        if (ch2EnvelopeCount <= 0) {

            envelopePeriod = mmu->readByte(SM2_ENVELOPE) & 7;
            envelopeDir = mmu->readByte(SM2_ENVELOPE) & 8;
            envelopeVol = mmu->readByte(SM2_ENVELOPE) >> 4;

            ch2EnvelopeCount = envelopePeriod;
            if (envelopePeriod) {
                if (envelopeDir && envelopeVol < 15)
                    mmu->writeByte(SM2_ENVELOPE, mmu->readByte(SM2_ENVELOPE) + 0x10);
                else if (!envelopeDir && envelopeVol > 0)
                    mmu->writeByte(SM2_ENVELOPE, mmu->readByte(SM2_ENVELOPE) - 0x10);
                else ch2EnvelopeEn = false;
            }
        }
    }

    //Channel 4 - Noise
    if (ch4EnvelopeEn) {
        ch4EnvelopeCount = (ch4EnvelopeCount - 1) & 0x7;
        if (ch4EnvelopeCount <= 0) {
            envelopePeriod = mmu->readByte(SM4_ENVELOPE) & 7;
            envelopeDir = mmu->readByte(SM4_ENVELOPE) & 8;
            envelopeVol = mmu->readByte(SM4_ENVELOPE) >> 4;

            ch4EnvelopeCount = envelopePeriod;
            if (envelopePeriod) {
                if (envelopeDir && envelopeVol < 15)
                    mmu->writeByte(SM4_ENVELOPE, mmu->readByte(SM4_ENVELOPE) + 0x10);
                else if (!envelopeDir && envelopeVol > 0)
                    mmu->writeByte(SM4_ENVELOPE, mmu->readByte(SM4_ENVELOPE) - 0x10);
                else ch4EnvelopeEn = false;
            }
        }
    }
}

int APU::calculateSweep() {
    u8 sweep = mmu->readByte(SM1_SWEEP);

    int freqShift =	ch1SweepShadow >> (sweep&7);
    int dir = (sweep &8) != 0;

    if (dir) freqShift *= -1;

    //look up subtrahand
    return ch1SweepShadow + freqShift;
}

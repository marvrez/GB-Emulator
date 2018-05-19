#ifndef APU_H
#define APU_H

#include "mmu.h"
#include "util.h"

#include <SFML/Audio.hpp>

#include <iostream>
#include <memory>
#include <stdint.h>


class APU {
    public:
        APU(std::shared_ptr<MMU>& mmu);
        void tick(Cycles cycle);
    private:
        inline void checkTrigger();
        inline void advanceSweep();
        inline void advanceLength();
        inline void advanceEnvelope();

        int calculateSweep();

        int cyclesPerSample = (FRAME_LENGTH / SAMPLES);

        std::shared_ptr<MMU> mmu;

        bool soundOn = false;

        int	framesyncCycle = 0;
        int	framesyncCount = 0;

        int	sampleCount	= cyclesPerSample;
        int	sampleIndex	= 0;

        bool ch1On = false;
        int	ch1DutyIndex = 0;
        int	ch1SweepCount = 0;
        bool ch1SweepEn	= false;
        bool ch1SweepDir = false;	// 0:Add / 1:Sub
        int	ch1SweepShadow = 0;
        bool ch1EnvolopeEn = false;
        int	ch1EnvelopeCount = 0;
        int	ch1Count = 0;

        bool ch2On = false;
        int	ch2DutyIndex = 0;
        bool ch2EnvelopeEn = false;
        int	ch2EnvelopeCount = 0;
        int	ch2Count = 0;

        bool ch3On	= false;
        int	ch3Position	= 0;
        int	ch3Count = 0;

        bool ch4On = false;
        int	ch4EnvelopeEn = false;
        int	ch4EnvelopeCount = 0;
        int	ch4Count = 0;

        s16 ch4Lsfr	= 0;

        s16 ch1Raw[SAMPLES] = {};
        s16 ch2Raw[SAMPLES] = {};
        s16 ch3Raw[SAMPLES] = {};
        s16 ch4Raw[SAMPLES] = {};

        sf::SoundBuffer ch1Buffer, ch2Buffer, ch3Buffer, ch4Buffer;
        sf::Sound ch1Sound, ch2Sound, ch3Sound, ch4Sound;
};

#endif

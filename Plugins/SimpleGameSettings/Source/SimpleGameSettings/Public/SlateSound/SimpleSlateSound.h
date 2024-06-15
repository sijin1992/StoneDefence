#pragma once

#include "CoreMinimal.h"
#include "Sound/ISlateSoundDevice.h"

struct FSlateSound;

class FSimpleSlateSoundDevice : public ISlateSoundDevice
{
public:
	virtual ~FSimpleSlateSoundDevice(){}

private:
	virtual void PlaySound(const FSlateSound& Sound, int32 UserIndex = 0) const override;//当点击或扫过Button后会触发PlaySound
	virtual float GetSoundDuration(const FSlateSound& Sound) const override;
};
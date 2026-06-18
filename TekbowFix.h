#pragma once
#include <API/ARK/Ark.h>

namespace TekBowFix
{
    float _cdecl Hook_AShooterCharacter_TakeDamage(AShooterCharacter* _this, float Damage, FDamageEvent* DamageEvent, 
                                                   AController* EventInstigator, AActor* DamageCauser);
}
#include "TekBowFix.h"

namespace TekBowFix
{
    DECLARE_HOOK(AShooterCharacter_TakeDamage, float, AShooterCharacter*, float, FDamageEvent*, AController*, AActor*);

    float _cdecl Hook_AShooterCharacter_TakeDamage(AShooterCharacter* _this, float Damage, FDamageEvent* DamageEvent, 
                                                   AController* EventInstigator, AActor* DamageCauser)
    {
        if (_this && DamageCauser)
        {
            // Comprobamos si la víctima es un jugador a pie (humano)
            if (_this->IsA(AShooterPlayerController::GetPrivateStaticClass()) || _this->IsA(APlayerCharacter::GetPrivateStaticClass()))
            {
                FString causerClassName;
                DamageCauser->GetClass()->GetName(&causerClassName);

                // Bloquea ProjTekBowArrow_Trank_C (Tranquilizante) y ProjTekBowArrow_Explosive_C (Fuego/Explosiva)
                if (causerClassName.Contains("ProjTekBowArrow_Trank") || causerClassName.Contains("ProjTekBowArrow_Explosive"))
                {
                    // Devolvemos 0 de daño y cortamos la aplicación de cualquier efecto en el jugador
                    return 0.0f;
                }
            }
        }

        // Si no cumple las condiciones, el juego procesa el daño de manera normal
        return AShooterCharacter_TakeDamage_original(_this, Damage, DamageEvent, EventInstigator, DamageCauser);
    }

    void Load()
    {
        Log::Get().Init("TekBowFix");
        Log::Get().Info("TekBowFix cargado con exito. Protegiendo a los jugadores del Tek Bow.");

        ArkApi::GetHooks().SetHook("AShooterCharacter.TakeDamage", &Hook_AShooterCharacter_TakeDamage, 
                                   &AShooterCharacter_TakeDamage_original);
    }

    void Unload()
    {
        ArkApi::GetHooks().DisableHook("AShooterCharacter.TakeDamage", &Hook_AShooterCharacter_TakeDamage);
    }
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        TekBowFix::Load();
        break;
    case DLL_PROCESS_DETACH:
        TekBowFix::Unload();
        break;
    }
    return TRUE;
}
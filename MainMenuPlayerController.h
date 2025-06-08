// Almas KASSYMBEKOV L2Q2
// Dernière modification: 10/04/2025

// Contrôleur personnalisé qui gère l’affichage
// du menu principal, la caméra de départ,
// les entrées clavier, et la bascule vers le jeu.

// Source d'où j'ai pris des parties du code:
// ChatGPT - OpenAI

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MainMenuPlayerController.generated.h"

// Forward declarations
class UUserWidget;
class ACameraActor;

// Classe qui remplace le PlayerController par défaut pour gérer le menu
UCLASS()
class MESHGENERATION_API AMainMenuPlayerController : public APlayerController
{
    GENERATED_BODY()

protected:
    // Appelé au lancement du jeu
    virtual void BeginPlay() override;

    // Initialise la caméra et affiche le menu
    UFUNCTION()
    void InitMainMenu();

    // Lance le jeu (caméra libre et suppression du menu)
    UFUNCTION(BlueprintCallable)
    void StartPlay();

public:
    // Caméra fixe utilisée pour le menu de départ
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
    ACameraActor* StartCamera;

    // Classe du widget du menu principal
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
    TSubclassOf<UUserWidget> MainMenuClass;

private:
    // Instance active du widget à l’écran
    UPROPERTY()
    UUserWidget* MainMenuWidget;
};
// Almas KASSYMBEKOV L2Q2
// Dernière modification: 10/04/2025

// Contrôleur personnalisé qui gère l’affichage
// du menu principal, la caméra de départ,
// les entrées clavier, et la bascule vers le jeu.

// Source d'où j'ai pris des parties du code:
// ChatGPT - OpenAI

#include "MainMenuPlayerController.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraActor.h"

// Appelée automatiquement au début du jeu
void AMainMenuPlayerController::BeginPlay()
{
    Super::BeginPlay();
    InitMainMenu(); // Initialiser le menu au début
    SetShowMouseCursor(true);
}

// Appelée lors du clic sur "Play" → passe en mode jeu
void AMainMenuPlayerController::StartPlay()
{
    // Si le menu existe, on le supprime de l'affichage
    if (MainMenuWidget)
    {
        MainMenuWidget->RemoveFromParent(); // Supprime le widget de la vue
        MainMenuWidget = nullptr;           // Libère la mémoire
    }

    SetShowMouseCursor(false);              // Cache le curseur
    SetInputMode(FInputModeGameOnly());     // Passe le contrôle au joueur (mode jeu)

    // Si un pawn (personnage) est présent, la caméra passe dessus
    if (GetPawn())
    {
        SetViewTarget(GetPawn());
    }
}


// Initialise le menu principal et la caméra de départ
void AMainMenuPlayerController::InitMainMenu()
{
    // Vérifie que le contrôleur est local (pas en multijoueur à distance)
    if (!IsLocalController()) return;

    // Recherche toutes les caméras de la scène
    TArray<AActor*> FoundCameras;
    UGameplayStatics::GetAllActorsOfClass(this, ACameraActor::StaticClass(), FoundCameras);

    // Si une caméra a été trouvée, on la définit comme vue principale
    if (FoundCameras.Num() > 0)
    {
        StartCamera = Cast<ACameraActor>(FoundCameras[0]); // Prend la première caméra trouvée
        SetViewTarget(StartCamera); // Définit cette caméra comme vue actuelle
    }

    // Si la classe du menu est bien assignée
    if (MainMenuClass)
    {
        // Crée une instance du widget de menu principal
        MainMenuWidget = CreateWidget<UUserWidget>(GetWorld(), MainMenuClass);
        if (MainMenuWidget)
        {
            MainMenuWidget->AddToViewport(); // Affiche le menu à l'écran

            FInputModeUIOnly InputMode; // Mode d'entrée UI uniquement
            InputMode.SetWidgetToFocus(MainMenuWidget->TakeWidget()); // Définit le focus sur le menu
            InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::LockAlways); // Verrouille la souris dans la fenêtre

            SetInputMode(InputMode);     // Applique le mode d'entrée UI
            bShowMouseCursor = true;     // Active l'affichage du curseur
        }
    }
}
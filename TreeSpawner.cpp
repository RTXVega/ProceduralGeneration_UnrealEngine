// Almas KASSYMBEKOV L2Q2
// Dernière modification: 07/04/2025

// Script permettant de générer des arbres aléatoirement sur le terrain, 
// en utilisant des paramètres spécifiques tels que la portée, la densité et la hauteur. 
// Il utilise la méthode Raycast : il s'agit de lancer de nombreux vecteurs depuis le ciel.
// Si la ligne atterrit sur un terrain vide et sans objet, l'arbre est généré ; sinon, l'arbre passe à l'emplacement suivant.
//
// Source où j'ai obtenu les parties du code :
// ChatCPT - OpenAI

#include "TreeSpawner.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Engine/World.h"

// Constructeur : initialise le composant d’arbres instanciés
ATreeSpawner::ATreeSpawner()
{
    PrimaryActorTick.bCanEverTick = false;

    InstancedComp = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("InstancedMesh"));
    RootComponent = InstancedComp;
}

// Appelée au démarrage du jeu
void ATreeSpawner::BeginPlay()
{
    Super::BeginPlay();
    ClearAndInit(); // Nettoie les instances existantes
}

// Fonction appelée pour relancer toute la génération d’arbres
void ATreeSpawner::RegenerateTrees()
{
    if (!TreeMesh || !TerrainActor) return;

    ClearAndInit();    // Supprime les anciens arbres
    SampleSurface();   // Génère de nouveaux emplacements
}

// Appelée dans l’éditeur à chaque modification
void ATreeSpawner::OnConstruction(const FTransform& Transform)
{
    Super::OnConstruction(Transform);
    // Vide ici : peut être utilisée si on veut régénérer en mode éditeur
}

// Supprime toutes les instances et configure le composant
void ATreeSpawner::ClearAndInit()
{
    InstancedComp->ClearInstances();
    InstancedComp->SetMobility(EComponentMobility::Static);
    InstancedComp->SetFlags(RF_Transactional);
}

// Fonction principale : recherche des points valides et place des arbres
void ATreeSpawner::SampleSurface()
{
    if (!TreeMesh || !TerrainActor) return;

    FVector Origin, Extent;
    TerrainActor->GetActorBounds(false, Origin, Extent);

    TArray<FVector> PlacedLocations; // Stocke les points déjà utilisés

    for (int i = 0; i < SampleCount; ++i)
    {
        float RandX = FMath::FRandRange(-Extent.X, Extent.X);
        float RandY = FMath::FRandRange(-Extent.Y, Extent.Y);

        FVector Start = Origin + FVector(RandX, RandY, RaycastHeight);
        FVector End = Start - FVector(0.f, 0.f, RaycastHeight * 2.f);

        FHitResult Hit;
        FCollisionQueryParams Params;
        Params.AddIgnoredActor(this);

        // Envoie un rayon vers le sol pour vérifier la surface
        if (GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, Params))
        {
            // Vérifie que le point est au-dessus du niveau de l’eau
            if (Hit.ImpactPoint.Z < WaterLevel)
                continue;

            // Vérifie la distance minimale entre arbres
            bool bTooClose = false;
            for (const FVector& OtherLocation : PlacedLocations)
            {
                if (FVector::DistSquared(OtherLocation, Hit.ImpactPoint) < FMath::Square(MinDistanceBetweenTrees))
                {
                    bTooClose = true;
                    break;
                }
            }

            if (bTooClose)
                continue;

            // Génère une échelle aléatoire uniforme
            float ScaleFactor = FMath::FRandRange(MinUniformScale, MaxUniformScale);
            FVector UniformScale = FVector(ScaleFactor);

            // Applique une rotation aléatoire sur l’axe Yaw
            FRotator RandomRotation = FRotator(0.f, FMath::FRandRange(0.f, 360.f), 0.f);

            // Ajuste la hauteur pour légèrement enterrer l’arbre
            FVector AdjustedImpact = Hit.ImpactPoint - FVector(0.f, 0.f, SinkDepth);

            // Convertit en coordonnées locales par rapport au composant
            FVector LocalLocation = InstancedComp->GetComponentTransform().InverseTransformPosition(AdjustedImpact);

            // Création de l’instance d’arbre avec transform complet
            FTransform InstanceTransform;
            InstanceTransform.SetLocation(LocalLocation);
            InstanceTransform.SetScale3D(UniformScale);
            InstanceTransform.SetRotation(FQuat(RandomRotation));

            // Ajoute l’arbre à la scène
            InstancedComp->AddInstance(InstanceTransform);
            PlacedLocations.Add(Hit.ImpactPoint);
        }
    }
}


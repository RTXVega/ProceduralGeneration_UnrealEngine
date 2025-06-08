// Almas KASSYMBEKOV L2Q2
// Dernière modification: 07/04/2025

// Script permettant de générer des arbres aléatoirement sur le terrain, 
// en utilisant des paramètres spécifiques tels que la portée, la densité et la hauteur. 
// Il utilise la méthode Raycast : il s'agit de lancer de nombreux vecteurs depuis le ciel.
// Si la ligne atterrit sur un terrain vide et sans objet, l'arbre est généré ; sinon, l'arbre passe à l'emplacement suivant.
//
// Source où j'ai obtenu les parties du code :
// ChatCPT - OpenAI


#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "TreeSpawner.generated.h"

UCLASS()
class MESHGENERATION_API ATreeSpawner : public AActor
{
    GENERATED_BODY()

public:
    ATreeSpawner(); // Constructeur

    // Nombre total d’essais de placement (densité d’arbres)
    UPROPERTY(EditAnywhere, Category = "Tree Spawner")
    int32 SampleCount = 500;

    // Hauteur à partir de laquelle les rayons sont tirés vers le sol
    UPROPERTY(EditAnywhere, Category = "Tree Spawner")
    float RaycastHeight = 1000.f;
    
    // Échelle minimale possible pour un arbre
    UPROPERTY(EditAnywhere, Category = "Tree Spawner")
    float MinUniformScale = 0.8f;

    // Échelle maximale possible pour un arbre
    UPROPERTY(EditAnywhere, Category = "Tree Spawner")
    float MaxUniformScale = 1.2f;
    
    // Distance minimale à respecter entre deux arbres
    UPROPERTY(EditAnywhere, Category = "Tree Spawner")
    float MinDistanceBetweenTrees = 150.0f;
    
    // Profondeur d’enfouissement de l’arbre dans le sol
    UPROPERTY(EditAnywhere, Category = "Tree Spawner")
    float SinkDepth = 5.0f;

    // Mesh utilisé pour représenter un arbre (à instancier)
    UPROPERTY(EditAnywhere, Category = "Tree Spawner")
    UStaticMesh* TreeMesh;

    // Référence vers le terrain cible pour les raycasts
    UPROPERTY(EditAnywhere, Category = "Tree Spawner")
    AActor* TerrainActor;
    
    // Niveau de l’eau : en dessous, aucun arbre ne sera placé
    UPROPERTY(EditAnywhere, Category = "Tree Spawner")
    float WaterLevel = 10.0f;
    
    // Fonction exposée à Blueprint pour relancer la génération
    UFUNCTION(BlueprintCallable, Category = "Spawning")
    void RegenerateTrees();

protected:
    // Appelée automatiquement dans l’éditeur
    virtual void OnConstruction(const FTransform& Transform) override;

    // Appelée au début du jeu
    virtual void BeginPlay() override;
    
private:
    // Composant pour stocker et dessiner les instances d’arbres
    UPROPERTY()
    UInstancedStaticMeshComponent* InstancedComp;

    // Supprime les anciennes instances et prépare le composant
    void ClearAndInit();

    // Cherche les points valides et instancie les arbres
    void SampleSurface();
};
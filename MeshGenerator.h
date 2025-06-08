// Almas KASSYMBEKOV L2Q2
// Dernière modification: 05/04/2025

// Déclaration d’un acteur Unreal permettant de
// générer un terrain procédural via un maillage.
// Paramétrable depuis l’éditeur, et compatible
// avec la génération procédurale d’arbres.

// Source d'où j'ai pris des parties du code:
// fettis - Terrain Generation in C++
// https://youtu.be/sNZ2g4qah28?si=0x07OzyqwC9w6lSS
// ChatGPT - OpenAI

#pragma once

#include "CoreMinimal.h"
#include "ProceduralMeshComponent.h"
#include "GameFramework/Actor.h"
#include "MeshGenerator.generated.h"

class UProceduralMeshComponent;
class UMaterialInterface;
class ATreeSpawner;

// Classe principale qui génère le terrain procédural
UCLASS()
class MESHGENERATION_API AMeshGenerator : public AActor
{
    GENERATED_BODY()

public:
    AMeshGenerator(); // Constructeur

    // Taille du terrain en X (nombre de cases)
    UPROPERTY(EditAnywhere, Meta = (ClampMin = 0))
    int XSize = 0;

    // Taille du terrain en Y
    UPROPERTY(EditAnywhere, Meta = (ClampMin = 0))
    int YSize = 0;

    // Hauteur maximale des sommets (amplitude verticale)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ClampMin = 0))
    float ZMultiplier = 1.0f;

    // Distance entre les sommets (échelle horizontale)
    UPROPERTY(EditAnywhere, Meta = (ClampMin = 0.000001))
    float Scale = 100.0f;

    // Fréquence du bruit de Perlin (inversée pour plus de clarté)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ClampMin = 0))
    float NoiseScale = 1.0f;

    // Graine aléatoire (Seed) pour la génération pseudo-aléatoire
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ClampMin = 0))
    int Seed = 0;

    // Nombre d’octaves à appliquer dans le Perlin Noise
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ClampMin = 1))
    int Octaves = 1;

    // Fréquence entre les octaves (détails du bruit)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ClampMin = 1.0f))
    float Lacunarity = 2.0f;

    // Échelle UV pour le matériau
    UPROPERTY(EditAnywhere, Meta = (ClampMin = 0.000001))
    float UVScale = 0;

    // Référence vers un spawner d’arbres (pour régénération automatique)
    UPROPERTY(EditAnywhere, Category = "Tree Spawning")
    ATreeSpawner* LinkedTreeSpawner;

protected:
    // Appelé au lancement du jeu
    virtual void BeginPlay() override;

    // Appelé dans l’éditeur après chaque changement de paramètre
    virtual void OnConstruction(const FTransform& Transform) override;

    // Matériau utilisé pour le terrain
    UPROPERTY(EditAnywhere)
    UMaterialInterface* Material;

    // Fonction exposée à Blueprint pour déclencher la génération avec paramètres
    UFUNCTION(BlueprintCallable, Category = "Generation")
    void GenerateTerrain(int InSeed, float InScale, float InZMultiplier, int InOctaves, float InLacunarity);

public:
    // Tick désactivé (pas utilisé)
    virtual void Tick(float DeltaTime) override;

private:
    // Composant du maillage procédural
    UProceduralMeshComponent* ProceduralMesh;

    // Liste des sommets
    TArray<FVector> Vertices;

    // Liste des triangles (index de sommets)
    TArray<int> Triangles;

    // Coordonnées UV
    TArray<FVector2D> UV0;

    // Normales pour l’éclairage
    TArray<FVector> Normals;

    // Tangentes pour le rendu correct
    TArray<struct FProcMeshTangent> Tangents;

    // Génère les sommets du maillage
    void CreateVertices();

    // Génère les triangles reliant les sommets
    void CreateTriangles();
};
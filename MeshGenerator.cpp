// Almas KASSYMBEKOV L2Q2
// Dernière modification: 05/04/2025

// Génère un terrain procédural en C++ via maillage
// Utilise Perlin Noise pour créer une carte de hauteur,
// avec contrôle de la graine, l'échelle, les octaves, etc.
// Peut également déclencher la génération d’arbres liés.

// Source d'où j'ai pris des parties du code:
// fettis - Terrain Generation in C++
// https://youtu.be/sNZ2g4qah28?si=0x07OzyqwC9w6lSS
// ChatGPT - OpenAI


#include "MeshGenerator.h"
#include "TreeSpawner.h"
#include "ProceduralMeshComponent.h"
#include "KismetProceduralMeshLibrary.h"

// Constructeur : initialise le composant de maillage procédural
AMeshGenerator::AMeshGenerator()
{
    PrimaryActorTick.bCanEverTick = false;

    ProceduralMesh = CreateDefaultSubobject<UProceduralMeshComponent>("Procedural Mesh");
    ProceduralMesh->SetupAttachment(GetRootComponent());
}

// Fonction principale appelée pour générer le terrain avec les paramètres donnés
void AMeshGenerator::GenerateTerrain(int InSeed, float InScale, float InZMultiplier, int InOctaves, float InLacunarity)
{
    // Enregistrement des paramètres
    Seed = InSeed;
    NoiseScale = InScale;
    ZMultiplier = InZMultiplier;
    Octaves = InOctaves;
    Lacunarity = InLacunarity;

    // Réinitialise les anciennes données du mesh
    Vertices.Reset();
    Triangles.Reset();
    UV0.Reset();

    // Génération des sommets et des triangles
    CreateVertices();
    CreateTriangles();

    // Calcule les normales et tangentes pour un rendu réaliste (lumière)
    UKismetProceduralMeshLibrary::CalculateTangentsForMesh(Vertices, Triangles, UV0, Normals, Tangents);

    // Crée la section de maillage avec les données calculées
    ProceduralMesh->CreateMeshSection(0, Vertices, Triangles, Normals, UV0, TArray<FColor>(), Tangents, true);

    // Applique le matériau choisi
    ProceduralMesh->SetMaterial(0, Material);

    // Si un spawner d’arbres est lié, déclenche sa régénération après un petit délai
    if (LinkedTreeSpawner && !IsRunningGame())
    {
        FTimerHandle DummyHandle;
        GetWorld()->GetTimerManager().SetTimer(
            DummyHandle,
            [this]()
            {
                if (LinkedTreeSpawner)
                {
                    LinkedTreeSpawner->RegenerateTrees();
                }
            },
            0.01f,
            false
        );
    }
}

// Appelé automatiquement dans l’éditeur à chaque modification — ici, désactivé
void AMeshGenerator::OnConstruction(const FTransform& Transform)
{
    Super::OnConstruction(Transform);
    // Ne fait rien ici (tout se passe dans GenerateTerrain)
}

// Démarrage du jeu : supprime l’ancienne section de mesh
void AMeshGenerator::BeginPlay()
{
    Super::BeginPlay();
    ProceduralMesh->ClearAllMeshSections();
}

// Tick désactivé (pas utilisé ici)
void AMeshGenerator::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

// Génère les sommets du terrain en utilisant le bruit de Perlin
void AMeshGenerator::CreateVertices()
{
    FRandomStream RandomStream(Seed); // Génère des offsets pseudo-aléatoires
    float OffsetX = RandomStream.FRandRange(-10000.f, 10000.f);
    float OffsetY = RandomStream.FRandRange(-10000.f, 10000.f);

    for (int X = 0; X <= XSize; ++X)
    {
        for (int Y = 0; Y <= YSize; ++Y)
        {
            float Amplitude = 1.0f;
            float Frequency = 1.0f / FMath::Max(NoiseScale, 0.0001f);
            float PerlinValue = 0.0f;

            // Application des octaves pour ajouter du détail
            for (int i = 0; i < Octaves; ++i)
            {
                float SampleX = (X + OffsetX) * Frequency;
                float SampleY = (Y + OffsetY) * Frequency;
                float Noise = FMath::PerlinNoise2D(FVector2D(SampleX, SampleY));
                PerlinValue += Noise * Amplitude;

                Amplitude *= 0.5f;
                Frequency *= Lacunarity;
            }

            // Hauteur finale multipliée
            float Z = PerlinValue * ZMultiplier;

            // Enregistrement du sommet
            Vertices.Add(FVector(X * Scale, Y * Scale, Z));

            // Coordonnées UV pour le matériau
            UV0.Add(FVector2D(X * UVScale, Y * UVScale));
        }
    }
}

// Connecte les sommets entre eux pour former des triangles
void AMeshGenerator::CreateTriangles()
{
    int Vertex = 0;
    for (int X = 0; X < XSize; ++X)
    {
        for (int Y = 0; Y < YSize; ++Y)
        {
            // Triangle 1
            Triangles.Add(Vertex);
            Triangles.Add(Vertex + 1);
            Triangles.Add(Vertex + YSize + 1);

            // Triangle 2
            Triangles.Add(Vertex + 1);
            Triangles.Add(Vertex + YSize + 2);
            Triangles.Add(Vertex + YSize + 1);

            ++Vertex;
        }
        ++Vertex; // Saut de ligne (maillage en grille)
    }
}

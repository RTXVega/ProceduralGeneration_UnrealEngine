// Almas KASSYMBEKOV L2Q2
// Ce test vérifie que le système de génération de terrain procédural (MeshGenerator) fonctionne correctement avec Unreal Automation Tool

// Sources ou j'ai pris des parties du code: ChatGPT - OpenAI

#include "Misc/AutomationTest.h"
#include "Engine/World.h"
#include "EngineUtils.h"
#include "MeshGenerator.h"

// Déclaration d'un test simple d'automatisation dans l'éditeur Unreal
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FMeshGeneratorBasicTest, "Procedural.MeshGenerator.BasicGeneration", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FMeshGeneratorBasicTest::RunTest(const FString& Parameters)
{
    // Obtient le monde actuel
    UWorld* World = GWorld;
    if (!World)
    {
        AddError(TEXT("World is null")); // Erreur si aucun monde
        return false;
    }

    // Crée dynamiquement un acteur MeshGenerator dans le monde
    AMeshGenerator* MeshGen = World->SpawnActor<AMeshGenerator>();
    TestNotNull(TEXT("MeshGenerator is valid"), MeshGen); // Vérifie qu'il a bien été créé

    // Paramètres de génération du terrain
    MeshGen->XSize = 200;         // Largeur
    MeshGen->YSize = 200;         // Longueur
    MeshGen->Scale = 100.f;       // Échelle
    MeshGen->Seed = 42;           // Graine aléatoire
    MeshGen->Octaves = 3;         // Détail du bruit
    MeshGen->Lacunarity = 2.0f;   // Fréquence du bruit
    MeshGen->ZMultiplier = 50.f;  // Hauteur du terrain

    // Appelle la génération de terrain avec les paramètres donnés
    MeshGen->GenerateTerrain(42, 1.0f, 50.0f, 3, 2.0f);

    // Vérifie que les sommets et triangles ont bien été générés
    TestTrue(TEXT("Vertices generated"), MeshGen->GetVertices().Num() > 0);
    TestTrue(TEXT("Triangles generated"), MeshGen->GetTriangles().Num() > 0);
    
    // Affiche dans la console le nombre de sommets
    UE_LOG(LogTemp, Warning, TEXT("Vertices: %d"), MeshGen->GetVertices().Num());

    return true; // Test réussi
}

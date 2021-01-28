#include "Pickup.h"

#include "Inventory.h"
#include "Components/SphereComponent.h"
#include "InventorySystemCPP/InventorySystemCPPCharacter.h"

// Sets default values
APickup::APickup()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Collision"));
	RootComponent = SphereCollision;

	SphereMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Sphere Mesh"));
	SphereMesh->SetupAttachment(SphereCollision);

	SphereCollision->InitSphereRadius(90.0f);
}

// Called when the game starts or when spawned
void APickup::BeginPlay()
{
	Super::BeginPlay();

	SphereCollision->OnComponentBeginOverlap.AddDynamic(this, &APickup::OnComponentBeginOverlap);
}

void APickup::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AInventorySystemCPPCharacter* Player = Cast<AInventorySystemCPPCharacter>(OtherActor);
	if(IsValid(Player))
	{
		if(Player->Inventory->AddItem(ItemToAdd, AmountToAdd, AmountToAdd))
		{
			OnPickup();
			if(AmountToAdd == 0)
			{
				Destroy();
			}
		}
	}
}
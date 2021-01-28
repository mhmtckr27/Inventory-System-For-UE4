#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Pickup.generated.h"

class USphereComponent;
UCLASS()
class INVENTORYSYSTEMCPP_API APickup : public AActor
{
	GENERATED_BODY()

	private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Components", meta = (AllowPrivateAccess = "true"))
	USphereComponent* SphereCollision;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* SphereMesh;
	public:	
	// Sets default values for this actor's properties
	APickup();

	protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Pickup", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class AItemBase> ItemToAdd;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Pickup", meta = (AllowPrivateAccess = "true"))
	int AmountToAdd;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UFUNCTION()
	void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	public:
	UFUNCTION(BlueprintImplementableEvent)
	void OnPickup();
};

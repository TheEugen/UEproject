// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/EngineTypes.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "UObject/UObjectGlobals.h"

#include "Item.h"
//#include "RPG_cppCharacter.h"

#include "Item_Weapon.generated.h"


// forward
class ARPG_cppCharacter;

UCLASS()
class RPG_CPP_API AItem_Weapon : public AItem
{
public:
	enum AttachPoint
	{
		Right = 0,
		Left = 1,
		Back = 2,
	};

	struct AttachInfo
	{
		bool inUse;
		AItem_Weapon::AttachPoint attach_point;
		AItem_Weapon* weapon;

		AttachInfo(bool b, AttachPoint ap, AItem_Weapon* w)
		{
			inUse = b;
			attach_point = ap;
			weapon = w;
		}
	};

private:
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	USkeletalMeshComponent* skeletal_mesh;

	UPROPERTY(EditAnywhere)
	UCapsuleComponent* capsule_collision;

	// attach points this weapon can be attached to
	TArray<AItem_Weapon::AttachPoint> attach_points;

	AItem_Weapon::AttachPoint current_attach_point;

	bool isEquipped;

	FName equip_socket_name;

	bool isDropping;

	AItem_Weapon* spawned_actor_ref;

public:
	AItem_Weapon(const FObjectInitializer& OI);

	void attachWeaponToCharacter(FName socket_name);

	//FName getSocketNameByPoint(AttachPoint attach_point);

	AttachPoint getSocketPointByName(FName socket_name);
	
	TArray<AttachPoint>& getAttachPoints();

	bool getIsEquipped();

	void equip();

	void unequip();
};

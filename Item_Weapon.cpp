// Fill out your copyright notice in the Description page of Project Settings.


#include "Item_Weapon.h"

#include "RPG_cppCharacter.h"


bool AItem_Weapon::getIsEquipped()
{
	return isEquipped;
}

void AItem_Weapon::equip()
{
	ARPG_cppCharacter* pc = (ARPG_cppCharacter*) GetWorld()->GetFirstPlayerController()->GetCharacter();
	
	if( pc )
	{
		// actor spawn parameters
		FActorSpawnParameters spawn_params = FActorSpawnParameters();
		spawn_params.bNoFail = true;

		// get the blueprint
		UClass* weapon_class = StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("/Game/ThirdPersonCPP/Items/Weapons/Item_Sword.Item_Sword_C"));
		
		// spawn actor
		AItem_Weapon* weapon = GetWorld()->SpawnActor<AItem_Weapon>(weapon_class, pc->GetActorLocation(), FRotator(0.f), spawn_params);
		
		// we dont want overlap events with equipped weapons
		weapon->capsule_collision->SetGenerateOverlapEvents(false);

		// attach actor to socket
		FAttachmentTransformRules rules = FAttachmentTransformRules::SnapToTargetIncludingScale;
		rules.bWeldSimulatedBodies = true;
		weapon->AttachToComponent(pc->GetMesh(), rules, weapon->equip_socket_name);
		
		// save ref to spawned actor for unequip later
		spawned_actor_ref = weapon;

		isEquipped = true;
	}
}

void AItem_Weapon::unequip()
{
	ARPG_cppCharacter* pc = (ARPG_cppCharacter*) GetWorld()->GetFirstPlayerController()->GetCharacter();

	if( pc && spawned_actor_ref )
	{	
		// destroy the spawned actor
		FDetachmentTransformRules rules = FDetachmentTransformRules::KeepRelativeTransform;
		spawned_actor_ref->DetachFromActor(rules);
		spawned_actor_ref->Destroy();

		isEquipped = false;
	}
}

TArray<AItem_Weapon::AttachPoint>& AItem_Weapon::getAttachPoints()
{
	return attach_points;
}

/*
FName AItem_Weapon::getSocketNameByPoint(AttachPoint attach_point)
{
	FName socket_name;

	switch( attach_point )
	{
	case AItem_Weapon::AttachPoint::Right:
		socket_name = FName("S_WEAPON_RDY_RIGHT");
		break;
	case AItem_Weapon::AttachPoint::Left:
		socket_name = FName("S_WEAPON_IDLE_LEFT");
		break;
	case AItem_Weapon::AttachPoint::Back:
		socket_name = FName("S_WEAPON_IDLE_BACK");
		break;
	default:
		UE_LOG(LogTemp, Fatal, TEXT("Item_Weapon getSocketNameByPoint: attach point type invalid"));
	}

	return socket_name;
}*/

AItem_Weapon::AttachPoint AItem_Weapon::getSocketPointByName(FName socket_name)
{
	TArray<FName> socket_names = { "S_WEAPON_RDY_RIGHT", "S_WEAPON_IDLE_LEFT", "S_WEAPON_IDLE_BACK"};
	
	if( socket_name.IsEqual(socket_names[0]) )
		return AItem_Weapon::AttachPoint::Right;
	else if( socket_name.IsEqual(socket_names[1]) )
		return AItem_Weapon::AttachPoint::Left;
	else if( socket_name.IsEqual(socket_names[2]) )
		return AItem_Weapon::AttachPoint::Back;
	else
		UE_LOG(LogTemp, Fatal, TEXT("Item_Weapon getSocketNameByPoint: attach point type invalid"));

	return AItem_Weapon::AttachPoint::Right;
}

void AItem_Weapon::attachWeaponToCharacter(FName socket_name)
{
	ARPG_cppCharacter* pc = (ARPG_cppCharacter*) GetOwner();
	if( pc )
	{
		if( pc->GetMesh()->DoesSocketExist(socket_name) )
		{
			FAttachmentTransformRules rules = FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true);
			rules.SnapToTargetIncludingScale = FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true);
			AttachToComponent(pc->GetMesh(), rules, socket_name);
			current_attach_point = getSocketPointByName(socket_name);
		}
	}
}


AItem_Weapon::AItem_Weapon(const FObjectInitializer& OI) : Super(OI)
{
	skeletal_mesh = CreateDefaultSubobject<USkeletalMeshComponent>(FName("skeletal mesh"));
	skeletal_mesh->SetMobility(EComponentMobility::Movable);
	skeletal_mesh->SetCollisionProfileName(FName("OverlapAll"));
	skeletal_mesh->SetEnableGravity(true);
	skeletal_mesh->SetupAttachment(root, FName("SkeletalMesh"));

	capsule_collision = CreateDefaultSubobject<UCapsuleComponent>(FName("CapsuleCollision"));
	capsule_collision->SetGenerateOverlapEvents(true);
	capsule_collision->SetCollisionProfileName("OverlapAllDynamic");
	capsule_collision->BodyInstance.SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	capsule_collision->SetupAttachment(root, FName("CapsuleCollision"));

	attach_points.Add(AItem_Weapon::AttachPoint::Left);
	attach_points.Add(AItem_Weapon::AttachPoint::Right);
	
	equip_socket_name = "S_WEAPON_RDY_RIGHT";
}
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

UENUM(BlueprintType)
enum EItemCategory
{
    Consumable	UMETA(DisplayName = "Consumable"),
	Readable	UMETA(DisplayName = "Readable"),
	Quest		UMETA(DisplayName = "Quest"),
	Equipment	UMETA(DisplayName = "Equipment"),
};

// Fill out your copyright notice in the Description page of Project Settings.


#include "GASCharacter.h"
#include "GASAttributeSet.h"
#include "GASAbilitySystemComponent.h"

// Sets default values
AGASCharacter::AGASCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AbilitySystemComponent = CreateDefaultSubobject<UGASAbilitySystemComponent>("AbilitySystemComp");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
}

// Called when the game starts or when spawned
void AGASCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	//Bindings for Attribute Change Delegates
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSetVar->GetHealthAttribute()).AddUObject(this, &ThisClass::OnHealthChangedNative);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSetVar->GetManaAttribute()).AddUObject(this, &ThisClass::OnManaChangedNative);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSetVar->GetBaseAttackDamageAttribute()).AddUObject(this, &ThisClass::OnBaseAttackDamageChangedNative);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSetVar->GetSpeedMultiplierAttribute()).AddUObject(this, &ThisClass::OnSpeedMultiplierChangedNative);
	
}

// Called every frame
void AGASCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AGASCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

UAbilitySystemComponent* AGASCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AGASCharacter::OnHealthChangedNative(const FOnAttributeChangeData& Data)
{
	OnHealthChange(Data.OldValue, Data.NewValue);
}

void AGASCharacter::OnManaChangedNative(const FOnAttributeChangeData& Data)
{
	OnManaChange(Data.OldValue, Data.NewValue);
}

void AGASCharacter::OnBaseAttackDamageChangedNative(const FOnAttributeChangeData& Data)
{
	OnBaseAttackDamageChange(Data.OldValue, Data.NewValue);
}

void AGASCharacter::OnSpeedMultiplierChangedNative(const FOnAttributeChangeData& Data)
{
	OnSpeedMultiplierChange(Data.OldValue, Data.NewValue);
}

//Function Definition for Alternate Method for Attribute Change Delegates
//void AGASCharacter::OnHealthChangedNative(const FOnAttributeChangeData& Data)
//{
//	OnHealthChanged(Data.OldValue, Data.NewValue);
//}
//
//void AGASCharacter::OnManaChangedNative(const FOnAttributeChangeData& Data)
//{
//	OnManaChanged(Data.OldValue, Data.NewValue);
//}

void AGASCharacter::InitializeAbility(TSubclassOf<UGameplayAbility> AbilityToGet, int32 AbilityLevel)
{
	if (HasAuthority() && AbilitySystemComponent)
	{
		AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(AbilityToGet, AbilityLevel, 0));
	}
}

void AGASCharacter::InitializeAbilityMulti(TArray<TSubclassOf<UGameplayAbility>> AbilitiesToAcquire, int32 AbilityLevel)
{
	for (TSubclassOf<UGameplayAbility> AbilitItem : AbilitiesToAcquire)
	{
		InitializeAbility(AbilitItem, AbilityLevel);
	}
}

void AGASCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	InitializeAbilityMulti(InitialAbilities, 0);
}

void AGASCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
}

void AGASCharacter::GetHealthValues(float& Health, float& MaxHealth)
{
	if (AttributeSetVar)
	{
		Health = AttributeSetVar->GetHealth();
		MaxHealth = AttributeSetVar->GetMaxHealth();
	}
}

void AGASCharacter::GetManaValues(float& Mana, float& MaxMana)
{
	if (AttributeSetVar)
	{
		Mana = AttributeSetVar->GetMana();
		MaxMana = AttributeSetVar->GetMaxMana();
	}
}

void AGASCharacter::GetBaseAttackDamageValues(float& BaseAttackDamage)
{
	if (AttributeSetVar)
	{
		BaseAttackDamage = AttributeSetVar->GetBaseAttackDamage();
	}
}

void AGASCharacter::GetSpeedMultiplierValues(float& SpeedMultiplier)
{
	if (AttributeSetVar)
	{
		SpeedMultiplier = AttributeSetVar->GetSpeedMultiplier();
	}
}

void AGASCharacter::RemoveAbilityWithTags(FGameplayTagContainer TagContainer)
{
	TArray<FGameplayAbilitySpec*> MatchingAbilities;
	AbilitySystemComponent->GetActivatableGameplayAbilitySpecsByAllMatchingTags(TagContainer, MatchingAbilities, true);
	for (FGameplayAbilitySpec* Spec : MatchingAbilities)
	{
		AbilitySystemComponent->ClearAbility(Spec->Handle);
	}
}

void AGASCharacter::ChangeAbilityLevelWithTags(FGameplayTagContainer TagContainer, int32 NewLevel)
{
	TArray<FGameplayAbilitySpec*> MatchingAbility;
	AbilitySystemComponent->GetActivatableGameplayAbilitySpecsByAllMatchingTags(TagContainer, MatchingAbility, true);
	for (FGameplayAbilitySpec* Spec : MatchingAbility)
	{
		Spec->Level = NewLevel;
	}
}

void AGASCharacter::CancelAbilityWithTags(FGameplayTagContainer WithTags, FGameplayTagContainer WithoutTags)
{
	AbilitySystemComponent->CancelAbilities(&WithTags, &WithoutTags, nullptr);
}

void AGASCharacter::AddLooseGameplayTag(FGameplayTag TagToAdd)
{
	GetAbilitySystemComponent()->AddLooseGameplayTag(TagToAdd);
	GetAbilitySystemComponent()->SetTagMapCount(TagToAdd, 1);
}

void AGASCharacter::RemoveLooseGameplayTags(FGameplayTag TagsToRemove)
{
	GetAbilitySystemComponent()->RemoveLooseGameplayTag(TagsToRemove);
}
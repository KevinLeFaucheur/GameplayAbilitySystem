


#include "UI/WidgetController/AttributeMenuWidgetController.h"

#include "AbilitySystem/ARPGAbilitySystemComponent.h"
#include "AbilitySystem/ARPGAttributeSet.h"
#include "AbilitySystem/Data/AttributeInfo.h"
#include "Player/CharacterPlayerState.h"

void UAttributeMenuWidgetController::BindCallbacksToDependencies()
{	
	check(AttributeInfo);	
	for (auto& Pair : GetARPGAttributeSet()->TagsToAttributes)
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Pair.Value).AddLambda(
        	[this, Pair](const FOnAttributeChangeData& Data)
        	{
        		BroadcastAttributeInfo(Pair.Key, Pair.Value);
        	}
        );
	}
	
	GetCharacterPlayerState()->OnAttributePointsChangedDelegate.AddLambda(
		[this] (int32 Points)
	{
		AttributePointsChangedDelegate.Broadcast(Points);
	});
	CharacterPlayerState->OnSpellPointsChangedDelegate.AddLambda(
		[this] (int32 Points)
	{
		SpellPointsChangedDelegate.Broadcast(Points);
	});
}

void UAttributeMenuWidgetController::BroadcastInitialValues()
{
	UARPGAttributeSet* AS = CastChecked<UARPGAttributeSet>(AttributeSet);
	check(AttributeInfo);	
	for (auto& Pair : AS->TagsToAttributes)
	{
		BroadcastAttributeInfo(Pair.Key, Pair.Value);
	}
	AttributePointsChangedDelegate.Broadcast(GetCharacterPlayerState()->GetAttributePoints());
	SpellPointsChangedDelegate.Broadcast(CharacterPlayerState->GetSpellPoints());
}

void UAttributeMenuWidgetController::UpgradeAttribute(const FGameplayTag& AttributeTag)
{
	UARPGAbilitySystemComponent* ARPGASC = CastChecked<UARPGAbilitySystemComponent>(AbilitySystemComponent);
	ARPGASC->UpgradeAttribute(AttributeTag);
}

void UAttributeMenuWidgetController::BroadcastAttributeInfo(const FGameplayTag& AttributeTag, const FGameplayAttribute& Attribute) const
{
	FARPGAttributeInfo Info = AttributeInfo->FindAttributeInfoForTag(AttributeTag);
	Info.AttributeValue = Attribute.GetNumericValue(AttributeSet);
	AttributeInfoDelegate.Broadcast(Info);
}

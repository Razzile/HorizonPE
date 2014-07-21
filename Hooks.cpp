#include <substrate.h>

#define Hook(x,y,z) MSHookFunction(MSFindSymbol(NULL, x), (void*)y, (void**)&z)
#define Sym(x) MSFindSymbol(NULL, x)
__attribute__((constructor)) void InitHooks();

struct Abilities {
	bool test1;
	bool test2;
	bool test3;
	bool test4;
};

float (*GetWalkingSpeedModifier)(void *self);
void (*Hurt)(void *self);

void (*Mob_JumpFromGround)(void *self, float height);

float (*HatchetItem_GetDestroySpeed)(void *ItemInstance, void *tile);
float (*PickaxeItem_GetDestroySpeed)(void *ItemInstance, void *tile);
float (*Item_GetDestroySpeed)(void *ItemInstance, void *tile);
float (*DiggerItem_GetDestroySpeed)(void *ItemInstance, void *tile);
float (*ShearsItem_GetDestroySpeed)(void *ItemInstance, void *tile);
float (*WeaponItem_GetDestroySpeed)(void *ItemInstance, void *tile);

void (*SurvivalMode_InitAbilities)(Abilities&);


namespace Hooks 
{
	namespace Player 
	{
		float GetWalkingSpeedModifier(void *self)
		{
			return 2.0f;
		}

		void Hurt(void *self)
		{
			// empty for no implementation
		}
	}
	namespace Mob
	{
		void JumpFromGround(void *self)
		{
			float *dataPtr = (float*)self;
			dataPtr[0x4C/sizeof(float)] = 2.0f;
		}
	}
	namespace Items
	{
		float HatchetItem_GetDestroySpeed(void *ItemInstance, void *tile)
		{
			return 100.0f;
		}
		float PickaxeItem_GetDestroySpeed(void *ItemInstance, void *tile)
		{
			return 100.0f;
		}
		float Item_GetDestroySpeed(void *ItemInstance, void *tile)
		{
			return 100.0f;
		}
		float DiggerItem_GetDestroySpeed(void *ItemInstance, void *tile)
		{
			return 100.0f;
		}
		float ShearsItem_GetDestroySpeed(void *ItemInstance, void *tile)
		{
			return 100.0f;
		}
		float WeaponItem_GetDestroySpeed(void *ItemInstance, void *tile)
		{
			return 100.0f;
		}
	}
	namespace SurvivalMode
	{
		void InitAbilities(void *self, Abilities *abilities)
		{
			int *test = (int*)abilities;
			test[0] = 12345678;
		}
	}
}

void InitHooks()
{
	using namespace Hooks;

	Hook("__ZN6Player23getWalkingSpeedModifierEv", Player::GetWalkingSpeedModifier, GetWalkingSpeedModifier);
	Hook("__ZN6Player4hurtEP6Entityi", Player::Hurt, Hurt);

	Hook("__ZN3Mob14jumpFromGroundEv", Mob::JumpFromGround, Mob_JumpFromGround);

	Hook("__ZN11HatchetItem15getDestroySpeedEP12ItemInstanceP4Tile", Items::HatchetItem_GetDestroySpeed, HatchetItem_GetDestroySpeed);
	Hook("__ZN11PickaxeItem15getDestroySpeedEP12ItemInstanceP4Tile", Items::HatchetItem_GetDestroySpeed, HatchetItem_GetDestroySpeed);
	Hook("__ZN4Item15getDestroySpeedEP12ItemInstanceP4Tile", Items::Item_GetDestroySpeed, Item_GetDestroySpeed);
	Hook("__ZN10DiggerItem15getDestroySpeedEP12ItemInstanceP4Tile", Items::DiggerItem_GetDestroySpeed, DiggerItem_GetDestroySpeed);
	Hook("__ZN10ShearsItem15getDestroySpeedEP12ItemInstanceP4Tile", Items::ShearsItem_GetDestroySpeed, ShearsItem_GetDestroySpeed);
	Hook("__ZN10WeaponItem15getDestroySpeedEP12ItemInstanceP4Tile", Items::WeaponItem_GetDestroySpeed, WeaponItem_GetDestroySpeed);

	Hook("__ZN12SurvivalMode13initAbilitiesER9Abilities", SurvivalMode::InitAbilities, SurvivalMode_InitAbilities);

}
#include <substrate.h>
#include "Settings.h"

#define Hook(x,y,z) MSHookFunction(MSFindSymbol(NULL, x), (void*)y, (void**)&z)
#define Sym(x) MSFindSymbol(NULL, x)
__attribute__((constructor)) void InitHooks();

/* definition of player abilities */
struct Abilities {
	bool invulnerable;
	bool flying;
	bool mayfly;
	bool instabuild;
};

/* definition of original functions */
namespace Originals 
{
	float (*Player_GetWalkingSpeedModifier)(void *self);
	void (*Player_Hurt)(void *self, void *entity, int damage);
	void (*Player_normalTick)(void *self);

	void (*Mob_JumpFromGround)(void *self, float height);

	bool (*Entity_IsInWater)(void *self);
	bool (*Entity_IsInWaterOrRain)(void *self);

	float (*HatchetItem_GetDestroySpeed)(void *ItemInstance, void *tile);
	float (*PickaxeItem_GetDestroySpeed)(void *ItemInstance, void *tile);
	float (*Item_GetDestroySpeed)(void *ItemInstance, void *tile);
	float (*DiggerItem_GetDestroySpeed)(void *ItemInstance, void *tile);
	float (*ShearsItem_GetDestroySpeed)(void *ItemInstance, void *tile);
	float (*WeaponItem_GetDestroySpeed)(void *ItemInstance, void *tile);
}

/* our mod implementations */
namespace Hooks 
{
	namespace Player 
	{
		float GetWalkingSpeedModifier(void *self)
		{
			return horizonSettings["kSpeed"];
		}

		void Hurt(void *self, void *entity, int damage)
		{
			bool invulnerable = horizonSettings["kInvulnerable"];
			if(invulnerable)
			{
				return;
			}
			return Originals::Player_Hurt(self, entity, damage);
		}

		void normalTick(void *self)
		{
			bool fly = horizonSettings["kFly"];
			if(fly)
			{
				Abilities *dataPtr = (Abilities*)self;
				Abilities *abilities = &dataPtr[0xC6C/sizeof(Abilities)];
				abilities->mayfly = true;
			}
			return Originals::Player_normalTick(self);
		}
	}
	namespace Mob
	{
		void JumpFromGround(void *self)
		{
			float *dataPtr = (float*)self;
			dataPtr[0x4C/sizeof(float)] = horizonSettings["kJump"];
		}
	}
	namespace Entity 
	{
		bool IsInWater(void *self)
		{
			bool enabled = horizonSettings["kWater"];
			return (enabled == true) ? true : Originals::Entity_IsInWater(self);
		}

		bool IsInWaterOrRain(void *self)
		{
			bool enabled = horizonSettings["kWater"];
			return (enabled == true) ? true : Originals::Entity_IsInWaterOrRain(self);
		}
	}
	namespace Items
	{
		float HatchetItem_GetDestroySpeed(void *ItemInstance, void *tile)
		{
			return horizonSettings["kDestroy"];
		}
		float PickaxeItem_GetDestroySpeed(void *ItemInstance, void *tile)
		{
			return horizonSettings["kDestroy"];
		}
		float Item_GetDestroySpeed(void *ItemInstance, void *tile)
		{
			return horizonSettings["kDestroy"];
		}
		float DiggerItem_GetDestroySpeed(void *ItemInstance, void *tile)
		{
			return horizonSettings["kDestroy"];
		}
		float ShearsItem_GetDestroySpeed(void *ItemInstance, void *tile)
		{
			return horizonSettings["kDestroy"];
		}
		float WeaponItem_GetDestroySpeed(void *ItemInstance, void *tile)
		{
			return horizonSettings["kDestroy"];
		}
	}
}

void InitHooks()
{
	using namespace Hooks;
	using namespace Originals;

	Hook("__ZN6Player23getWalkingSpeedModifierEv", Player::GetWalkingSpeedModifier, Player_GetWalkingSpeedModifier);
	Hook("__ZN6Player4hurtEP6Entityi", Player::Hurt, Player_Hurt);
	Hook("__ZN11LocalPlayer10normalTickEv", Player::normalTick, Player_normalTick);

	Hook("__ZN3Mob14jumpFromGroundEv", Mob::JumpFromGround, Mob_JumpFromGround);

	Hook("__ZNK6Entity9isInWaterEv", Entity::IsInWater, Entity_IsInWater);
	Hook("__ZNK6Entity9isInWaterEv", Entity::IsInWaterOrRain, Entity_IsInWaterOrRain);

	Hook("__ZN11HatchetItem15getDestroySpeedEP12ItemInstanceP4Tile", Items::HatchetItem_GetDestroySpeed, HatchetItem_GetDestroySpeed);
	Hook("__ZN11PickaxeItem15getDestroySpeedEP12ItemInstanceP4Tile", Items::HatchetItem_GetDestroySpeed, HatchetItem_GetDestroySpeed);
	Hook("__ZN4Item15getDestroySpeedEP12ItemInstanceP4Tile", Items::Item_GetDestroySpeed, Item_GetDestroySpeed);
	Hook("__ZN10DiggerItem15getDestroySpeedEP12ItemInstanceP4Tile", Items::DiggerItem_GetDestroySpeed, DiggerItem_GetDestroySpeed);
	Hook("__ZN10ShearsItem15getDestroySpeedEP12ItemInstanceP4Tile", Items::ShearsItem_GetDestroySpeed, ShearsItem_GetDestroySpeed);
	Hook("__ZN10WeaponItem15getDestroySpeedEP12ItemInstanceP4Tile", Items::WeaponItem_GetDestroySpeed, WeaponItem_GetDestroySpeed);

	// Hook("__ZN12SurvivalMode13initAbilitiesER9Abilities", SurvivalMode::InitAbilities, SurvivalMode_InitAbilities);

}
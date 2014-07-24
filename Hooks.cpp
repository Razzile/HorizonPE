/*                   __  __ 
 *|__| _  _._  _  _ |__)|_  
 *|  |(_)| |/_(_)| )|   |__ 
 *                       
 * File: Hooks.cpp
 * Created by Razzile & AlphaMATTER
 */
#include <substrate.h>
#include "Settings.h"

#define Hook(x,y,z) MSHookFunction(MSFindSymbol(NULL, x), (void*)y, (void**)&z)
#define Sym(x) MSFindSymbol(NULL, x)

__attribute__((constructor)) void InitHooks();

#pragma mark Utilities
/* definition of player abilities */
struct Abilities {
	bool invulnerable;
	bool flying;
	bool mayfly;
	bool instabuild;
};

void *playerRef = NULL;
bool IsEntityPlayer(void *entityRef)
{
	return (entityRef == playerRef);
}

#pragma mark Originals
/* definition of original functions */
namespace Originals 
{
	namespace Player 
	{
		float (*GetWalkingSpeedModifier)(void *self);
		void (*Hurt)(void *self, void *entity, int damage);
		void (*normalTick)(void *self);
	}
	namespace Mob
	{
		void (*JumpFromGround)(void *self, float height);
		void (*ActuallyHurt)(void *self, int damage);
	}
	namespace Entity 
	{
		bool (*IsInWater)(void *self);
		bool (*IsInWaterOrRain)(void *self);
		bool (*IsUnderLiquid)(void *self, void *material);
	}

	float (*HatchetItem_GetDestroySpeed)(void *ItemInstance, void *tile);
	float (*PickaxeItem_GetDestroySpeed)(void *ItemInstance, void *tile);
	float (*Item_GetDestroySpeed)(void *ItemInstance, void *tile);
	float (*DiggerItem_GetDestroySpeed)(void *ItemInstance, void *tile);
	float (*ShearsItem_GetDestroySpeed)(void *ItemInstance, void *tile);
	float (*WeaponItem_GetDestroySpeed)(void *ItemInstance, void *tile);

	namespace GameMode 
	{
		void (*DestroyBlock)(void *self, void *player, int xco, int yco, int zco, char c);
	}
}

#pragma mark Hooks
/* our mod implementations */
namespace Hooks {

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
		return Originals::Player::Hurt(self, entity, damage);
	}

	void normalTick(void *self)
	{
		playerRef = self;
		bool fly = horizonSettings["kFly"];
		if (fly)
		{
			/* The players Abilites struct is located 0xC6C bytes from the main player stack */
			Abilities *dataPtr = (Abilities*)self;
			Abilities *abilities = &dataPtr[0xC6C/sizeof(Abilities)];
			abilities->mayfly = true;
		}
		return Originals::Player::normalTick(self);
	}
}
namespace Mob
{
	void JumpFromGround(void *self)
	{
		/* jump height is located 0x4C bytes up from Mob stack */
		/* see this picture to see what I mean */
		/* http://i.imgur.com/QeR9vPU.png */
		float *dataPtr = (float*)self;
		dataPtr[0x4C/sizeof(float)] = horizonSettings["kJump"];
	}

	void ActuallyHurt(void *self, int damage)
	{
		bool onehit = horizonSettings["kOnehit"];
		if (onehit)
		{
			damage = 99;
		}
		return Originals::Mob::ActuallyHurt(self, damage);
	}
}
namespace Entity 
{
	bool IsInWater(void *self)
	{
		bool enabled = horizonSettings["kWater"];
		return (enabled == true) ? true : Originals::Entity::IsInWater(self);
	}

	bool IsInWaterOrRain(void *self)
	{
		bool enabled = horizonSettings["kWater"];
		return (enabled == true) ? true : Originals::Entity::IsInWaterOrRain(self);
	}

	bool IsUnderLiquid(void *self, void *material)
	{
		bool scuba = horizonSettings["kScuba"];
		if(scuba)
		{
			if(IsEntityPlayer(self))
			{
				return false;
			}
		}
		return Originals::Entity::IsUnderLiquid(self, material);
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
namespace GameMode
{
	void DestroyBlock(void *self, void *player, int xco, int yco, int zco, char c)
	{
		bool nuker = horizonSettings["kNuker"];
		int size = horizonSettings["kNukerSize"];
		if(nuker)
		{
			for (int x = 0-size/2; x < size/2; x++)
			{
				for (int y = 0-size/2; y < size/2; y++)	
				{
					for (int z = 0-size/2; z < size/2; z++)
					{
						Originals::GameMode::DestroyBlock(self, player, xco+x, yco+y, zco+z, c);
						Originals::GameMode::DestroyBlock(self, player, xco+x, yco-y, zco+z, c);
					}
				}
			}
		}
		return Originals::GameMode::DestroyBlock(self, player, xco, yco, zco, c);
	}
}

} /* END HOOKS */

void InitHooks()
{
	using namespace Hooks;

	Hook("__ZN6Player23getWalkingSpeedModifierEv", Player::GetWalkingSpeedModifier, Originals::Player::GetWalkingSpeedModifier);
	Hook("__ZN6Player4hurtEP6Entityi", Player::Hurt, Originals::Player::Hurt);
	Hook("__ZN11LocalPlayer10normalTickEv", Player::normalTick, Originals::Player::normalTick);

	Hook("__ZN3Mob14jumpFromGroundEv", Mob::JumpFromGround, Originals::Mob::JumpFromGround);
	Hook("__ZN3Mob12actuallyHurtEi", Mob::ActuallyHurt, Originals::Mob::ActuallyHurt);

	Hook("__ZN6Entity13isUnderLiquidEPK8Material", Entity::IsUnderLiquid, Originals::Entity::IsUnderLiquid);

	Hook("__ZN11HatchetItem15getDestroySpeedEP12ItemInstanceP4Tile", Items::HatchetItem_GetDestroySpeed, Originals::HatchetItem_GetDestroySpeed);
	Hook("__ZN11PickaxeItem15getDestroySpeedEP12ItemInstanceP4Tile", Items::HatchetItem_GetDestroySpeed, Originals::HatchetItem_GetDestroySpeed);
	Hook("__ZN4Item15getDestroySpeedEP12ItemInstanceP4Tile", Items::Item_GetDestroySpeed, Originals::Item_GetDestroySpeed);
	Hook("__ZN10DiggerItem15getDestroySpeedEP12ItemInstanceP4Tile", Items::DiggerItem_GetDestroySpeed, Originals::DiggerItem_GetDestroySpeed);
	Hook("__ZN10ShearsItem15getDestroySpeedEP12ItemInstanceP4Tile", Items::ShearsItem_GetDestroySpeed, Originals::ShearsItem_GetDestroySpeed);
	Hook("__ZN10WeaponItem15getDestroySpeedEP12ItemInstanceP4Tile", Items::WeaponItem_GetDestroySpeed, Originals::WeaponItem_GetDestroySpeed);

	Hook("__ZN8GameMode12destroyBlockEP6Playeriiia", GameMode::DestroyBlock, Originals::GameMode::DestroyBlock);
}
/*
Sword Code taken from Sword of the Highlander tutorial.
http://webadvisor.aupr.edu/noc/Othertutorials/qdevels/-%20Sword%20Of%20The%20Highlander%20Revisited%20%20.html

Expanded upon and abstracted to created multiple weapons

Some comments are from tutorial


/*knife variable defs*/


#define KNIFE_NORMAL_DAMAGE 4
#define KNIFE_KICK 0
#define KNIFE_RANGE 10

/*sword variable definitions*/

#define SWORD_NORMAL_DAMAGE 6
#define SWORD_KICK 10
#define SWORD_DEATHMATCH_DAMAGE 200
#define SWORD_RANGE 15

/*
Axe variable defs
*/
#define AXE_NORMAL_DAMAGE 8
#define AXE_KICK 10
#define AXE_RANGE 20

/*
Lance Variable defs
*/

#define LANCE_NORMAL_DAMAGE 6
#define LANCE_KICK 5
#define LANCE_RANGE 40

/*
warhammer variables
*/
#define WHAMMER_NORMAL_DAMAGE 15
#define WHAMMER_KICK 20
#define WHAMMER_RANGE 25

/*
halberd 
*/

#define HALBERD_NORMAL_DAMAGE 10
#define HALBERD_KICK 10
#define HALBERD_RANGE 40


/*
rapier

*/

#define RAPIER_NORMAL_DAMAGE 10
#define RAPIER_KICK 10
#define RAPIER_RANGE 40

/*
morning star
*/

#define MSTAR_NORMAL_DAMAGE 25
#define MSTAR_KICK 15
#define MSTAR_RANGE 30

/*
BUSTER SWORD
*/


#define BSWORD_NORMAL_DAMAGE 200
#define BSWORD_KICK 300
#define BSWORD_RANGE 40



//fire_melee (ent, start, forward, damage, kick,range);
void fire_melee(edict_t *self, vec3_t start, vec3_t aimdir, int damage, int kick, int range)
{
	trace_t tr; //detect whats in front of you up to range "vec3_t end"
 
    vec3_t end;
 
    // Figure out what we hit, if anything:
 
    VectorMA (start, range, aimdir, end);  //calculates the range vector                      
 
    tr = gi.trace (self->s.origin, NULL, NULL, end, self, MASK_SHOT);
                        // figuers out what in front of the player up till "end"
    
   // Figure out what to do about what we hit, if anything
 
    if (!((tr.surface) && (tr.surface->flags & SURF_SKY)))    
    {
        if (tr.fraction < 1.0)        
        {  

            if (tr.ent->takedamage)            
            {
                T_Damage (tr.ent, self, self, aimdir, tr.endpos, tr.plane.normal, damage, kick, 0,0);
                gi.sound (self, CHAN_AUTO, gi.soundindex("misc/fhit3.wav") , 1, ATTN_NORM, 0); 
 
            }        
            else        
            {                
                gi.WriteByte (svc_temp_entity);    
                gi.WriteByte (TE_SPARKS);
                gi.WritePosition (tr.endpos);    
                gi.WriteDir (tr.plane.normal);
                gi.multicast (tr.endpos, MULTICAST_PVS);
 
                gi.sound (self, CHAN_AUTO, gi.soundindex("weapons/grenlb1b.wav") , 1, ATTN_NORM, 0);
 
            }    
        }
    }
    return;
}

void melee_attack(edict_t *ent, vec3_t g_offset, int damage, int kick, int range)
{
	vec3_t forward, right; 
	vec3_t start;
	vec3_t offset;

	/*if(is_quad)
		damage *= 4;*/
	AngleVectors (ent->client->v_angle, forward, right, NULL);
	VectorSet(offset, 24, 8, ent->viewheight-8);
	VectorAdd (offset, g_offset, offset);
	P_ProjectSource (ent->client, ent->s.origin, offset, forward, right, start);

	VectorScale (forward, -2, ent->client->kick_origin);
	ent->client->kick_angles[0] = -1;

	if(ent->client->quad_damage)
		damage *= 4;

	fire_melee (ent, start, forward, damage,kick,range);

}


void fire_spell(edict_t *ent, vec3_t g_offset, int spell)
{
	vec3_t forward, right; 
	vec3_t start;
	vec3_t offset;

	/*if(is_quad)
		damage *= 4;*/
	AngleVectors (ent->client->v_angle, forward, right, NULL);
	VectorSet(offset, 24, 8, ent->viewheight-8);
	VectorAdd (offset, g_offset, offset);
	P_ProjectSource (ent->client, ent->s.origin, offset, forward, right, start);

	VectorScale (forward, -2, ent->client->kick_origin);
	ent->client->kick_angles[0] = -1;

	if(spell == 0) /*force push*/
	{
		if(ent->client->mana >= 10)
		{
			fire_shotgun (ent, start, forward, 0, 100, 500, 500, DEFAULT_DEATHMATCH_SHOTGUN_COUNT, MOD_SHOTGUN);
			ent->client->mana -= 10;
		}
	}
	else if(spell == 1) /*grapple beam*/
	{
		if(ent->client->mana >= 20) //cm243 The grapple beam feels a little inaccurate to me. I'm not sure if there's a more reliable way to do it, but it feels hard to aim and still hit them
		{
			fire_shotgun (ent, start, forward, 0, -100, 500, 500, DEFAULT_DEATHMATCH_SHOTGUN_COUNT, MOD_SHOTGUN);
			fire_rail (ent, start, forward, 0, 0);
			ent->client->mana -= 20;
		}
	}
	else if(spell == 2) /*heal*/
	{
		if(ent->client->mana >= 20)
		{
			ent->health += 10;
			ent->client->mana -= 20;
		}
	}
	else if(spell == 3) /*bfg (safety dance)*/
	{
		if(ent->client->mana >= 100)
		{
			fire_bfg (ent, start, forward, 300, 400, 25);
			ent->client->mana -= 100;
		}
	}
	else if(spell == 4) /*double damage*/
	{
		if(ent->client->mana >= 50)
		{
			ent->client->quad_damage = true;
			ent->client->buffTimer = 1000;
			ent->client->mana -= 50;
		}
	}
	else if(spell == 5) /*drain soul*/
	{
		if(ent->client->mana >= 40)
		{
			trace_t tr;
 
			vec3_t end;

			VectorMA (start, 100, forward, end);                 
 
			tr = gi.trace (ent->s.origin, NULL, NULL, end, ent, MASK_SHOT);

			ent->client->mana -= 40;

			if (!((tr.surface) && (tr.surface->flags & SURF_SKY)))    
			{
				if (tr.fraction < 1.0)        
				{  

					if (tr.ent->takedamage)            
					{
						T_Damage (tr.ent, ent, ent, forward, tr.endpos, tr.plane.normal, 200, 0, 0,0);
						ent->health += 30;
						//gi.sound (ent, CHAN_AUTO, gi.soundindex("misc/fhit3.wav") , 1, ATTN_NORM, 0); 
					}        
					else        
					{                
						gi.WriteByte (svc_temp_entity);    
						gi.WriteByte (TE_SPARKS);
						gi.WritePosition (tr.endpos);    
						gi.WriteDir (tr.plane.normal);
						gi.multicast (tr.endpos, MULTICAST_PVS);
 
						//gi.sound (ent, CHAN_AUTO, gi.soundindex("weapons/grenlb1b.wav") , 1, ATTN_NORM, 0);
 
					}    
				}
			}
		}
	}

}


/* abstracted weapon specific attack funcitons with generic melee weapon attack functons.
   this will allow weapons to be created by only added a new fire and base weapon functon
*/
void Weapon_Sword_Fire(edict_t *ent)
{
	int damage;
	int kick;
	int range; 

	range = SWORD_RANGE;
	kick = SWORD_KICK;
	if(deathmatch->value)
		damage = SWORD_DEATHMATCH_DAMAGE;
	else
		damage = SWORD_NORMAL_DAMAGE;
	melee_attack (ent, vec3_origin, damage, kick, range);
	//gi.centerprintf(ent, "Sword attack");
	ent->client->ps.gunframe++;
}

void Weapon_Knife_Fire(edict_t *ent)
{
	int damage;
	int kick;
	int range;

	damage = KNIFE_NORMAL_DAMAGE;
	kick = KNIFE_KICK;
	range = KNIFE_RANGE;

	melee_attack (ent, vec3_origin, damage, kick ,range);
	ent->client->ps.gunframe++;
	//gi.centerprintf(ent, "lance attack");
}

void Weapon_Knife(edict_t *ent)
{
	static int      pause_frames[]  = {19, 32, 0};
	static int      fire_frames[]   = {5, 0};
 
	Weapon_Generic (ent, 4, 8, 52, 55, pause_frames, fire_frames, Weapon_Knife_Fire);
}
void Weapon_Sword (edict_t *ent)
{
	static int      pause_frames[]  = {19, 32, 0};
	static int      fire_frames[]   = {5, 0};
 
	Weapon_Generic (ent, 4, 8, 52, 55, pause_frames, fire_frames, Weapon_Sword_Fire);
}

void Weapon_Axe_Fire(edict_t *ent)
{
	int damage;
	int kick;
	int range;

	range = AXE_RANGE;
	kick = AXE_KICK;
	damage = AXE_NORMAL_DAMAGE;

	melee_attack (ent, vec3_origin, damage, kick, range);
	ent->client->ps.gunframe++;
	//gi.centerprintf(ent, "Axe attack");
}

void Weapon_Axe(edict_t *ent)
{
	static int	pause_frames[]	= {22, 28, 34, 0};
	static int	fire_frames[]	= {8, 9, 0};

	Weapon_Generic (ent, 7, 18, 36, 39, pause_frames, fire_frames, Weapon_Axe_Fire);
}

void Weapon_Lance_Fire(edict_t *ent)
{
	int damage;
	int kick;
	int range;

	damage = LANCE_NORMAL_DAMAGE;
	kick = LANCE_KICK;
	range = LANCE_RANGE;

	melee_attack (ent, vec3_origin, damage, kick ,range);
	ent->client->ps.gunframe++;
	//gi.centerprintf(ent, "lance attack");
}
void Weapon_Lance(edict_t *ent)
{
	static int	pause_frames[]	= {56, 0};
	static int	fire_frames[]	= {4, 0};

	Weapon_Generic (ent, 3, 18, 56, 61, pause_frames, fire_frames, Weapon_Lance_Fire);
}

void Weapon_WarHammer_Fire(edict_t *ent)
{
	int damage;
	int kick;
	int range;

	damage = WHAMMER_NORMAL_DAMAGE;
	kick = WHAMMER_KICK;
	range = WHAMMER_RANGE;

	melee_attack (ent, vec3_origin, damage, kick ,range);
	ent->client->ps.gunframe++;
}
void Weapon_WarHammer(edict_t *ent)
{
	static int	pause_frames[]	= {29, 42, 57, 0};
	static int	fire_frames[]	= {7, 0};

	Weapon_Generic (ent, 6, 17, 57, 61, pause_frames, fire_frames, Weapon_WarHammer_Fire);
}
void Weapon_Rapier_Fire(edict_t *ent)
{
	int damage;
	int kick;
	int range;

	damage = RAPIER_NORMAL_DAMAGE;
	kick = RAPIER_KICK;
	range = RAPIER_RANGE;

	melee_attack (ent, vec3_origin, damage, kick ,range);
	ent->client->ps.gunframe++;
}
void Weapon_Rapier(edict_t *ent)
{
	static int	pause_frames[]	= {0};
	static int	fire_frames[]	= {6, 7, 8, 9, 10, 11, 0};

	Weapon_Generic (ent, 5, 20, 49, 53, pause_frames, fire_frames, Weapon_Rapier_Fire);
}

void Weapon_MorningStar_Fire(edict_t *ent)
{
	int damage;
	int kick;
	int range;

	damage = MSTAR_NORMAL_DAMAGE;
	kick = MSTAR_KICK;
	range = MSTAR_RANGE;

	melee_attack (ent, vec3_origin, damage, kick ,range);
	ent->client->ps.gunframe++;
}
void Weapon_MorningStar(edict_t *ent)
{
	static int	pause_frames[]	= {29, 42, 57, 0};
	static int	fire_frames[]	= {7, 0};

	Weapon_Generic (ent, 6, 17, 57, 61, pause_frames, fire_frames, Weapon_MorningStar_Fire);
}

void Weapon_Halberd_Fire(edict_t *ent)
{
	int damage;
	int kick;
	int range;

	damage = HALBERD_NORMAL_DAMAGE;
	kick = HALBERD_KICK;
	range = HALBERD_RANGE;

	melee_attack (ent, vec3_origin, damage, kick ,range);
	ent->client->ps.gunframe++;
}
void Weapon_Halberd(edict_t *ent)
{
	static int	pause_frames[]	= {25, 33, 42, 50, 0};
	static int	fire_frames[]	= {5, 0};

	Weapon_Generic (ent, 4, 12, 50, 54, pause_frames, fire_frames, Weapon_Halberd_Fire);
}

void Weapon_BusterSword_Fire(edict_t *ent)
{
	int damage;
	int kick;
	int range;

	damage = BSWORD_NORMAL_DAMAGE;
	kick = BSWORD_KICK;
	range = BSWORD_RANGE;

	melee_attack (ent, vec3_origin, damage, kick ,range);
	ent->client->ps.gunframe++;
}
void Weapon_BusterSword(edict_t *ent)
{
	static int	pause_frames[]	= {39, 45, 50, 55, 0};
	static int	fire_frames[]	= {9, 17, 0};

	Weapon_Generic (ent, 8, 32, 55, 58, pause_frames, fire_frames, Weapon_BusterSword_Fire);
}

void Weapon_SpellBook_Fire(edict_t *ent)
{
	fire_spell(ent, vec3_origin, ent->client->spellNum);
	ent->client->ps.gunframe++;
}
void Weapon_SpellBook(edict_t *ent)
{
	static int	pause_frames[]	= {34, 51, 59, 0};
	static int	fire_frames[]	= {6, 0};

	Weapon_Generic (ent, 5, 16, 59, 64, pause_frames, fire_frames, Weapon_SpellBook_Fire);
}




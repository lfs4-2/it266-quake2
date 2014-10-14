/*
Sword Code taken from Sword of the Highlander tutorial.
http://webadvisor.aupr.edu/noc/Othertutorials/qdevels/-%20Sword%20Of%20The%20Highlander%20Revisited%20%20.html

Expanded up and abstracted to created multiple weapons

Some comments are from tutorial

*/
/*sword variable definitions*/

#define SWORD_NORMAL_DAMAGE 100
#define SWORD_DEATHMATCH_DAMAGE 150
#define SWORD_KICK 20
#define SWORD_RANGE 35

/*
Axe variable defs
*/
#define AXE_NORMAL_DAMAGE 200
#define AXE_KICK 200
#define AXE_RANGE 35

/*
Lance Variable defs
*/

#define LANCE_NORMAL_DAMAGE 35
#define LANCE_KICK 150
#define LANCE_RANGE 125

//created file 

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
			/*
				Merchant interaction test working!!
			*/
			if(strcmp(tr.ent->classname, "info_player_coop") == 0)
			{
				gi.centerprintf(self, "What are ya buyin");
			}
		

			//gi.centerprintf(self, tr.ent->classname);

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

	fire_melee (ent, start, forward, damage, kick,range);
}
/*void fire_sword ( edict_t *self, vec3_t start, vec3_t aimdir, int damage, int kick)
{    
    trace_t tr; //detect whats in front of you up to range "vec3_t end"
 
    vec3_t end;
 
    // Figure out what we hit, if anything:
 
    VectorMA (start, SWORD_RANGE, aimdir, end);  //calculates the range vector                      
 
    tr = gi.trace (self->s.origin, NULL, NULL, end, self, MASK_SHOT);
                        // figuers out what in front of the player up till "end"
    
   // Figure out what to do about what we hit, if anything
 
    if (!((tr.surface) && (tr.surface->flags & SURF_SKY)))    
    {
        if (tr.fraction < 1.0)        
        {            
            if (tr.ent->takedamage)            
            {
                //This tells us to damage the thing that in our path...hehe
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
	gi.centerprintf(self, "Sword Attack");
    return;
}  // 1-13-98 DanE
*/
/*
void sword_attack(edict_t *ent, vec3_t g_offset, int damage)
{
	vec3_t forward, right; 
	vec3_t start;
	vec3_t offset;

	AngleVectors (ent->client->v_angle, forward, right, NULL);
	VectorSet(offset, 24, 8, ent->viewheight-8);
	VectorAdd (offset, g_offset, offset);
	P_ProjectSource (ent->client, ent->s.origin, offset, forward, right, start);

	VectorScale (forward, -2, ent->client->kick_origin);
	ent->client->kick_angles[0] = -1;

	fire_sword (ent, start, forward, damage, SWORD_KICK);


}
*/

/*
void fire_axe(edict_t *self, vec3_t start, vec3_t aimdir, int damage, int kick)
{
	trace_t tr;

	vec3_t end;

	VectorMA(start, AXE_RANGE, aimdir, end);

	tr = gi.trace(self->s.origin, NULL,NULL, end, self, MASK_SHOT);

	if(!((tr.surface)&&(tr.surface->flags & SURF_SKY)))
	{
		if(tr.fraction <  1.0)
		{
			if(tr.ent->takedamage)
			{
				T_Damage(tr.ent, self, self, aimdir, tr.endpos, tr.plane.normal, damage, kick, 0, 0);
				gi.sound(self, CHAN_AUTO, gi.soundindex("misc/fhit3.wav"), 1, ATTN_NORM, 0);
			}
			else
			{
				gi.WriteByte(svc_temp_entity);
				gi.WriteByte(TE_SPARKS);
				gi.WritePosition(tr.endpos);
				gi.WriteDir(tr.plane.normal);
				gi.multicast(tr.endpos, MULTICAST_PVS);

				gi.sound (self, CHAN_AUTO, gi.soundindex("weapons/grenlb1b.wav"), 1, ATTN_NORM, 0 );
			}
		}
	}
	gi.centerprintf(self, "Axe Attack");
	return;
}

void axe_attack(edict_t *ent, vec3_t g_offset, int damage)
{
	vec3_t forward, right;
	vec3_t start;
	vec3_t offset;

	AngleVectors (ent->client->v_angle, forward, right, NULL);
	VectorSet(offset, 24, 8, ent->viewheight-8);
	VectorAdd (offset, g_offset, offset);
	P_ProjectSource (ent->client, ent->s.origin, offset, forward, right, start);

	VectorScale (forward, -2, ent->client->kick_origin);
	ent->client->kick_angles[0] = -1;

	fire_axe (ent, start, forward, damage, AXE_KICK);
}
*/

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

void Weapon_Sword (edict_t *ent)
{
	static int		pause_frames[] = {19,32,0};
	static int		fire_frames[] =  {5,0};


	Weapon_Generic (ent, 4,8,52,55,pause_frames, fire_frames, Weapon_Sword_Fire);
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
	static int		pause_frames[] = {19, 32, 0};
	static int		fire_frames[] = {5,0};

	Weapon_Generic (ent, 4,8,52,55,pause_frames, fire_frames, Weapon_Axe_Fire);
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
	static int		pause_frames[] = {19, 32, 0};
	static int		fire_frames[] = {5,0};

	Weapon_Generic (ent, 4,8,52,55,pause_frames, fire_frames, Weapon_Lance_Fire);
}
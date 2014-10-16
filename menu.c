/*
Code take from quake 2 menu tutorials
http://webadvisor.aupr.edu/noc/Othertutorials/Skunkworks/Generic%20Menuing%20System.htm
http://webadvisor.aupr.edu/noc/Othertutorials/Skunkworks/Menu%20Addon.htm
*/

#include "g_local.h"

void addLineToMenu(edict_t *ent, char *line, int option)
{
	if(ent->client->menustorage.menu_active)
		return;
	if(ent->client->menustorage.numLines > 12)
		return;

	ent->client->menustorage.numLines++;
	
	ent->client->menustorage.messages[ent->client->menustorage.numLines].msg = malloc(22);
	sprintf(ent->client->menustorage.messages[ent->client->menustorage.numLines].msg, "%-21s", line);
	ent->client->menustorage.messages[ent->client->menustorage.numLines].option=option;
}

void clearMenu(edict_t *ent)
{
	int i = 0;

	for (i = 0; i<12; i++)
	{
		ent->client->menustorage.messages[i].option = 0;
		if (ent->client->menustorage.messages[i].msg!=NULL)
		{
			free(ent->client->menustorage.messages[i].msg);
		}
	}

	ent->client->menustorage.optionSelected = NULL;
	ent->client->menustorage.currentLine = 0;
	ent->client->menustorage.numLines = 0;
}

void setMenuHandler (edict_t *ent, qboolean (*optionSelected)(edict_t *ent, int option))
{
	ent->client->menustorage.optionSelected = optionSelected;
}

void menuDown(edict_t *ent)
{
	int		i; 

	i = 1;

	do
	{
		if (ent->client->menustorage.currentLine<ent->client->menustorage.numLines)
			ent->client->menustorage.currentLine++;
		else
		{
		while ((ent->client->menustorage.messages[i].option==0)&(i<ent->client->menustorage.numLines))
			{i++;}
		ent->client->menustorage.currentLine=i;
		}
	}
	while (ent->client->menustorage.messages[ent->client->menustorage.currentLine].option==0);
	showMenu(ent);
}

void menuUp(edict_t *ent)
{
	int		i;

	i = ent->client->menustorage.numLines;

	i = ent->client->menustorage.numLines;
	do
	{
		if(ent->client->menustorage.numLines>2)
			ent->client->menustorage.currentLine--;
		else
		{
			while((ent->client->menustorage.messages[i].option==0) | (i < ent->client->menustorage.numLines))
				{i--;}
			ent->client->menustorage.currentLine =  i;
		}
	}
	while (ent->client->menustorage.messages[ent->client->menustorage.currentLine].option == 0);
		showMenu(ent);
}

void menuSelect (edict_t *ent)
{
	ent->client->menustorage.optionSelected(ent, ent->client->menustorage.messages[ent->client->menustorage.currentLine].option);

	/*
	Removed these lines according to the advanced menu tutorial
	ent->client->menustorage.messages[ent->client->menustorage.currentLine].option;
	closeMenu(ent);
	ent->client->menustorage.optionSelected(ent, i);
	ent->client->menustorage.optionSelected=NULL;
	*/
}

void initMenu (edict_t *ent)
{
	int i;

	for(i=0 ; i<12; i++)
	{
		ent->client->menustorage.menu_active = false;
		ent->client->menustorage.displaymsg;
		ent->client->menustorage.currentLine = 0;
		ent->client->menustorage.numLines = 0;
	}
}

void showMenu (edict_t * ent)
{
	int i, j;
	char finalMenu[1024];
	char tmp[80];


	sprintf (finalMenu, "xv 32 yv 8 picn inventory ");

	j = 48; //y coordinate

	for (i = 1; i < (ent->client->menustorage.numLines + 1); i++)
	{
		gi.cprintf(ent, PRINT_HIGH, "%1 \n", ent->client->menustorage.messages[i].option);
		if (ent->client->menustorage.messages[i].option == 0)
		{
			sprintf(tmp, "xv 52 yv %i string \" %s \" ", j, ent->client->menustorage.messages[i].msg );
		}
		else if(i == ent->client->menustorage.currentLine)
		{
			sprintf(tmp, "xv 52 yv %i string2 \">> %s <<\" ", j, ent->client->menustorage.messages[i].msg);
		}
		else 
			sprintf (tmp,"xv 52 yv %i string2 \"   %s   \" ",j,ent->client->menustorage.messages[i].msg);

		strcat(finalMenu,tmp);
		j += 12;
	}

	ent->client->menustorage.menu_active = true;

	ent->client->menustorage.displaymsg = false;
	ent->client->showinventory = false;
	ent->client->showscores = true;
	gi.WriteByte (svc_layout);
	gi.WriteString(finalMenu);
	gi.unicast(ent, true);
}

void closeMenu (edict_t *ent)
{
	ent->client->menustorage.optionSelected = NULL;

	clearMenu(ent);

	ent->client->showscores = false;
	ent->client->menustorage.menu_active = false;
	ent->client->menustorage.displaymsg = false;
	ent->client->showinventory = false;
}

void ChangeLine (edict_t *ent, int lineNum, char *line, int option)
{
	if (lineNum > (ent->client->menustorage.numLines))
		return;

	sprintf(ent->client->menustorage.messages[lineNum].msg, "%-21s", line);
	ent->client->menustorage.messages[lineNum].option = option;
}

qboolean testMenuHandler (edict_t *ent, int option)
{
	gitem_t *item;

	int cantbuy;
	

	int prices2[8] = {10, 20, 30, 35, 40 , 45, 60, 80};

	int prices[8] = {10, 20, 40, 50, 70 , 80, 90, 120};

	cantbuy = 1;

	//price = ent->client->menustorage.prices;

	switch(option)
	{
	case 1:
		{
			
			if(ent->client->gold >= prices[0])
			{
				ent->client->gold -= prices[0];
				item = FindItem("Sword");
				ent->client->pers.inventory[ITEM_INDEX(item)] = 2;
				cantbuy = 0;
			}
		}
		break;
	case 2: 
		{
			if(ent->client->gold >= prices[1])
			{
				ent->client->gold -= prices[1];
				item = FindItem("Axe");
				ent->client->pers.inventory[ITEM_INDEX(item)] = 3;
				cantbuy = 0;
			}
		}
		break;
	case 3:
	{
		if(ent->client->gold >= prices[2])
		{
			ent->client->gold -= prices[2];
			item = FindItem("Lance");
			ent->client->pers.inventory[ITEM_INDEX(item)] = 4;
			cantbuy = 0;
		}
	}
	break;
	case 4: 
	if(ent->client->gold >= prices[3])
	{
		ent->client->gold -= prices[3];
		item = FindItem("WarHammer");
		ent->client->pers.inventory[ITEM_INDEX(item)] = 5;
		cantbuy = 0;
	}
	break;
	case 5:
	{
		if(ent->client->gold >= prices[4])
		{
			ent->client->gold -= prices[4];
			item = FindItem("Halberd");
			ent->client->pers.inventory[ITEM_INDEX(item)] = 6;
			cantbuy = 0;
		}
	}
	break;
	case 6:
	{
		if(ent->client->gold >= prices[5])
		{
			ent->client->gold -= prices[5];
			item = FindItem("Rapier");
			ent->client->pers.inventory[ITEM_INDEX(item)] = 7;
			cantbuy = 0;
		}
	}
	break;
	case 7:
	{
		if(ent->client->gold >= prices[6])
		{
			ent->client->gold -= prices[6];
			item = FindItem("MorningStar");
			ent->client->pers.inventory[ITEM_INDEX(item)] = 8;
			cantbuy = 0;
		}
	}
	break;
	case 8:
	{
		if(ent->client->gold >= prices[7])
		{
			ent->client->gold -= prices[7];
			item = FindItem("BusterSword");
			ent->client->pers.inventory[ITEM_INDEX(item)] = 9;
			cantbuy = 0;
		}
	}
	break;
	case 9:
	{
			closeMenu(ent);
			cantbuy = 2;
			break;
	}
}
	if(cantbuy == 1)
	{
		closeMenu(ent);
		gi.centerprintf(ent, "Come back when you're not poor");
	}
	else if (cantbuy == 0)
	{
		gi.centerprintf(ent, "You got a %s", item->pickup_name);
	}


	return true;
}


extern void Menu_test(edict_t *ent)
{

	if(ent->client->showscores || ent->client->showinventory || ent->client->menustorage.menu_active)
		return;

	clearMenu(ent);
	addLineToMenu(ent, "Welcome traveler", 0);
	addLineToMenu(ent, "What are ya buy'n?", 0);
	
	addLineToMenu(ent, "Buy Sword: 10g", 1);
	/*addLineToMenu(ent, line, 0);*/
	addLineToMenu(ent, "Buy Axe: 20g", 2);
	addLineToMenu(ent, "Buy Lance: 40g", 3);
	addLineToMenu(ent, "Buy Warhammer: 50g", 4);
	addLineToMenu(ent, "Buy Halberd: 70g", 5);
	addLineToMenu(ent, "Buy Rapier: 80g", 6);
	addLineToMenu(ent, "Buy MorningStar: 90g", 7);
	addLineToMenu(ent, "Buy BusterSword: 120g", 8);
	addLineToMenu(ent, "Exit",9 );
	
	setMenuHandler (ent, testMenuHandler);
	ent->client->menustorage.currentLine = 3;
	showMenu(ent);

}

void clearAllMenus (edict_t *ent)
{
	int i;

	for(i = 0; i < game.maxclients; i++)
	{
		ent =  g_edicts + 1+ i;
		closeMenu(ent);
	}
}


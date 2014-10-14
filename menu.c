/*
Code take from quake 2 menu tutorials
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
	sprintf (ent->client->menustorage.messages[ent->client->menustorage.numLines].msg, "%-21s", line);
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

void setMenuHandler (edict_t *ent, void (*optionSelected)(edict_t *ent, int option))
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
			while((ent->client->menustorage.messages[i].option==0) | i < ent->client->menustorage.numLines)
				{i--;}
			ent->client->menustorage.currentLine =  i;
		}
	}
	while (ent->client->menustorage.messages[ent->client->menustorage.currentLine].option == 0);
		showMenu(ent);
}

void menuSelect (edict_t *ent)
{
	int i;


	i = ent->client->menustorage.messages[ent->client->menustorage.currentLine].option;
	closeMenu(ent);
	ent->client->menustorage.optionSelected(ent, i);
	ent->client->menustorage.optionSelected=NULL;
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
	char tmp[80], tmp2[80];


	sprintf(finalMenu, "xv 32 yv 8 picn inventory");

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
			sprintf(tmp,"xv 52 yv %i string2 \"   %s   \" ",j,ent->client->menustorage.messages[i].msg);

		strcat(finalMenu, tmp);
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
	clearMenu(ent);

	ent->client->showscores = false;
	ent->client->menustorage.menu_active = false;
	ent->client->menustorage.displaymsg = false;
	ent->client->showinventory = false;
}

void testMenuHandler (edict_t *ent, int option)
{
	switch(option)
	{
	case 1: 
		gi.centerprintf(ent, "option1\n");
		break;
	case 2:
		gi.centerprintf(ent, "option2\n");
		break;
	case 3: 
		gi.centerprintf(ent, "option3\n");
		break;
	default:
		gi.centerprintf(ent, "fuckup\n");
	}
}


void Menu_test(edict_t *ent)
{
	if(ent->client->showscores || ent->client->showinventory || ent->client->menustorage.menu_active)
		return;

	clearMenu(ent);
	addLineToMenu(ent, "option 1", 1);
	addLineToMenu(ent, "option 2", 1);
	addLineToMenu(ent, "misc text", 1);
	addLineToMenu(ent, "option 3", 1);
	setMenuHandler (ent, testMenuHandler);
	ent->client->menustorage.currentLine = 2;
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
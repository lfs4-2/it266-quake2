/*
code borrowed from quake 2 tutorials
*/

void addLineToMenu(edict_t *ent, char *line, int option);
void clearMenu(edict_t *ent);
void setMenuHandler(edict_t *ent, void (*optionSelected)(edict_t *ent, int option));

void menuUp(edict_t *ent);
void menuDown (edict_t *ent);
void menuSelect (edict_t *ent);

void initMenu (edict_t *ent);
void showMenu (edict_t *ent);
void closeMenu (edict_t *ent);

void MenuTest (edict_t *ent);

typedef struct menuMsg_s
{
	char *msg;
	int option;
}menuMsg_t;

typedef struct menuSystem_s
{
	qboolean	menu_active;
	qboolean	displaymsg;
	qboolean	(*optionSelected)(edict_t *ent, int option);
	menuMsg_t	messages[12];
	int			currentLine;
	int			numLines;
}menuSystem_t;
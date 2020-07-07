#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <termios.h>
#include <stdlib.h>
#include "intcode.h"
#include "printu.h"

int64_t intcode_prog[] = {
#include "inputs/input25"
};

struct termios stored_settings;
void setup_term()
{
	tcgetattr(0, &stored_settings);
	struct termios new_settings = stored_settings;
	new_settings.c_lflag &= (~ICANON);
	new_settings.c_lflag &= (~ECHO); // don't echo the character

	tcsetattr(0, TCSANOW, &new_settings);
}

#define MAX_X 99
#define MAX_Y 99
char area[MAX_Y + 1][MAX_X + 1] = { [0 ... MAX_Y] = { [0 ... MAX_X] = ' ' } };
int pos_x = 50;
int pos_y = 10;

void put_str(struct intcode_machine * m, char * s)
{
	while (*s) PUT(m->inputs, *(s++));
}

#define UNKNOWN 0
#define GROUND 1
#define HOLDING 2
char * items[20] = { [0 ... 19] = NULL };
int itemstate[20] = { [0 ... 19] = UNKNOWN };

char * item_blacklist[] = {
	"infinite loop",
	"giant electromagnet",
};

int find_item(char * s)
{
	int i = 0;
	while (items[i] != NULL) {
		if (!strcmp(s, items[i])) return i;
		i++;
	}
	items[i] = s;
	return i;
}

int first_item_on_ground()
{
	int i = 0;
	while (items[i] != NULL) {
		if (itemstate[i] == GROUND) {
			return i;
		}
		i++;
	}
	return -1;
}

char * get_output(struct intcode_machine * m)
{
	char * output = malloc(1024);
	int i = 0;

	run_machine(m);
	while (!EMPTY(m->outputs)) {
		output[i++] = GET(m->outputs);
	}
	output[i] = '\0';
	//m->outputs.read = 0;
	//m->outputs.write = 0;

	return output;
}

void parse_room(char * output)
{
	area[pos_y - 1][pos_x - 1] = '#';
	area[pos_y - 1][pos_x + 1] = '#';
	area[pos_y + 1][pos_x - 1] = '#';
	area[pos_y + 1][pos_x + 1] = '#';
	area[pos_y][pos_x - 1] = '#';
	area[pos_y][pos_x + 1] = '#';
	area[pos_y + 1][pos_x] = '#';
	area[pos_y - 1][pos_x] = '#';

	if (strstr(output, "you are ejected back to the checkpoint")) {
		pos_x -= 2;
		printf("Ejected back!\n");
		return;
	}
	char * tmp;
	tmp = strstr(output, "Doors here lead:\n");
	if (tmp) {
		tmp += sizeof("Doors here lead:");
		char * dir;
		int n = 0;
		while (1 == sscanf(tmp, "- %ms\n%n", &dir, &n)) {
			printf("door: |%s|\n", dir);
			if (!strcmp(dir, "north")) { area[pos_y - 1][pos_x] = '-'; }
			if (!strcmp(dir, "south")) { area[pos_y + 1][pos_x] = '-'; }
			if (!strcmp(dir, "west")) { area[pos_y][pos_x - 1] = '|'; }
			if (!strcmp(dir, "east")) { area[pos_y][pos_x + 1] = '|'; }
			tmp += n;
		}
	}


	printU("\e[1;1H\e[2J");
	for (int y = 0; y <= 35; y++) {
		for (int x = 0; x < MAX_X; x++) {
			printfU("%c", area[y][x]);
		}
		printf("\n");
	}
	printfU("\e[%d;%dH@", pos_y + 1, pos_x + 1);
	printfU("\e[%d;%dH", 20, 1);

	// deal with items
	for (int item = 0; items[item] != NULL; item++) {
		itemstate[item] = UNKNOWN;
	}
	tmp = strstr(output, "Items here:\n");
	if (tmp) {
		tmp += sizeof("Items here:");
		char * it;
		int n = 0;
		while (1 == sscanf(tmp, "- %m[^\n]\n%n", &it, &n)) {
		//	printf("item: |%s|\n", it);
			if (!strcmp(it, "giant electromagnet")) {
				break;
			}
			area[pos_y][pos_x] = it[0];
			int item = find_item(it);
			itemstate[item] = GROUND;
			tmp += n;
		}
	}

	tmp = strstr(output, "Doors here lead:\n");
	//if (tmp) tmp[-1] = '\0';
	printf("%s", output);

}

void check_items(struct intcode_machine * m)
{
	put_str(m, "inv\n");

	char * output = get_output(m);
	char * tmp = strstr(output, "Items in your inventory:\n");
	if (tmp) {
		tmp += sizeof("Items in your inventory:");

		char * it;
		int n = 0;
		while (1 == sscanf(tmp, "- %m[^\n]\n%n", &it, &n)) {
			int item = find_item(it);
			itemstate[item] = HOLDING;
			tmp += n;
		}
	} else {
		printU("\n");
	}

	for (int item = 0; items[item] != NULL; item++) {
		printf("%d) %d %s\n", item, itemstate[item], items[item]);
	}
	free(output);
}

void take_first_item_on_ground(struct intcode_machine * m)
{
	int it = first_item_on_ground();
	if (it == -1) return;

	put_str(m, "take ");
	put_str(m, items[it]);
	put_str(m, "\n");
	area[pos_y][pos_x] = 'X';
	char * s = get_output(m);
	printf(s);
	check_items(m);
}

int main(int argc, char * argv[])
{
	setup_term();
	struct intcode_machine * m;
	m = NEW_INTCODE_MACHINE_EMEM(intcode_prog, _I(), 10);

	parse_room(get_output(m));

	put_str(m, "east\n");
	parse_room(get_output(m));
	put_str(m, "east\n");
	parse_room(get_output(m));
	take_first_item_on_ground(m); // semiconductor
	put_str(m, "north\n");
	parse_room(get_output(m));
	take_first_item_on_ground(m); // planetoid
	put_str(m, "north\n");
	parse_room(get_output(m));
	take_first_item_on_ground(m); // antenna
	put_str(m, "south\n");
	parse_room(get_output(m));
	put_str(m, "west\n");
	parse_room(get_output(m));
	take_first_item_on_ground(m); // food rations
	put_str(m, "west\n");
	parse_room(get_output(m));
	put_str(m, "west\n");
	parse_room(get_output(m));
	take_first_item_on_ground(m); // monolith
	put_str(m, "east\n");
	parse_room(get_output(m));
	put_str(m, "east\n");
	parse_room(get_output(m));
	put_str(m, "north\n");
	parse_room(get_output(m));
	take_first_item_on_ground(m); // brochure
	put_str(m, "east\n");
	parse_room(get_output(m));
	take_first_item_on_ground(m); // jam
	put_str(m, "west\n");
	parse_room(get_output(m));
	put_str(m, "north\n");
	parse_room(get_output(m));
	put_str(m, "north\n");
	parse_room(get_output(m));
	take_first_item_on_ground(m); // weather machine
	put_str(m, "south\n");
	parse_room(get_output(m));
	put_str(m, "south\n");
	parse_room(get_output(m));
	put_str(m, "south\n");
	parse_room(get_output(m));
	put_str(m, "east\n");
	parse_room(get_output(m));
	put_str(m, "south\n");
	parse_room(get_output(m));
	put_str(m, "east\n");
	parse_room(get_output(m));
	put_str(m, "south\n");
	parse_room(get_output(m));
	take_first_item_on_ground(m);
	put_str(m, "south\n");
	parse_room(get_output(m));
	take_first_item_on_ground(m);

	while (1) {
		char c;
		read(STDIN_FILENO, &c, 1);
		if (c == 'k') {
			if (area[pos_y - 1][pos_x] == '#') continue;
			put_str(m, "north\n");
			pos_y -= 2;
			parse_room(get_output(m));
			check_items(m);
		}
		if (c == 'j') {
			if (area[pos_y + 1][pos_x] == '#') continue;
			put_str(m, "south\n");
			pos_y += 2;
			parse_room(get_output(m));
			check_items(m);
		}
		if (c == 'h') {
			if (area[pos_y][pos_x - 1] == '#') continue;
			put_str(m, "west\n");
			pos_x -= 2;
			parse_room(get_output(m));
			check_items(m);
		}
		if (c == 'l') {
			if (area[pos_y][pos_x + 1] == '#') continue;
			put_str(m, "east\n");
			pos_x += 2;
			parse_room(get_output(m));
			check_items(m);
		}
		if (c == 'q') { break; }
		if (c == 't') {
			take_first_item_on_ground(m);
		}
		if (c == 'i') {
			check_items(m);
		}
		if (c == 'd') {
			put_str(m, "drop monolith\n");
		}
		if (c == 's') { // start dropping/picking items until we can get thru!
			int nitems = 0;
			while (items[nitems] != NULL) {
				nitems++;
			}

			for (int i = 0; i < 1024; i++) {

				for (int j = 0; j < nitems; j++) {
					int t = 1 << j;
					if (i % t == 0) { // then swap item j
						if (itemstate[j] == GROUND) {
							put_str(m, "take ");
							itemstate[j] = HOLDING;
						} else {
							put_str(m, "drop ");
							itemstate[j] = GROUND;

						}

						put_str(m, items[j]);
						put_str(m, "\n");

						char * slask = get_output(m);
						printf("%s\n", slask);
						free(slask);
					}
				}
				check_items(m);

				// try to go east with this item configuration
				put_str(m, "east\n");
				char * s = get_output(m);
				if (!strstr(s, "heavier") && !strstr(s, "lighter")) {
					printf("COOL! %i\n", i);
					printf("%s\n", s);
					check_items(m);
					break;
				} else {
					printf("Ejected!\n");
				}
				printf("Done? %i\n", i);
			}

			check_items(m);

		}
		if (c > '0' && c < '9') {

		}
	}
	tcsetattr(0, TCSANOW, &stored_settings);

	return 0;
}

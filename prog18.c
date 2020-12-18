#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "intcode.h"

// XXX This works but way too inefficient.
#define SIZE_X 82
#define SIZE_Y 81
char input[(SIZE_X) * SIZE_Y];
struct node * first_node;

struct edge_list {
	struct edge_list * next;
	struct edge_list * prev;
	struct edge * edge;
};

struct edge {
	struct node * node1;
	struct node * node2;
	int weight;
};

struct node {
	char type;
	struct node * next;
	struct node * prev;
	struct edge_list * first_edge;
};

#define UP (-SIZE_X)
#define DOWN (SIZE_X)
#define LEFT -1
#define RIGHT 1

struct node * node91 = NULL;
int print_edges(struct node * node, int detail)
{

	struct edge_list * e = node->first_edge;
	// for each pair of edges in node, replace with one
	printf("%p %c: ", node, node->type);
	int count = 0;
	while (e) {
		if (detail)
			printf(" (%c %d %c)", e->edge->node1->type, e->edge->weight, e->edge->node2->type);
		count++;
		e = e->next;
	}

	printf("... %d\n", count);
	return count;
}

void print_nodes(struct node * n)
{
	while (n) {
		struct node * next = n->next;
		print_edges(n, 1);
		n = next;
	}
}

struct edge * find_edge(struct node * node1, struct node * node2)
{
	struct edge_list * e = node1->first_edge;
	while (e) {
		if (e->edge->node1 == node2 && e->edge->node2 == node1) {
			return e->edge;
		}
		if (e->edge->node2 == node2 && e->edge->node1 == node2) {
			return e->edge;
		}
		e = e->next;
	}
	return NULL;

}

void connect_nodes(struct node * node1, struct node * node2, int weight)
{
	// check if these nodes are already connected
	struct edge * cur_edge1 = find_edge(node1, node2);
	struct edge * cur_edge2 = find_edge(node2, node1);
	if (cur_edge1 || cur_edge2) {
		//printf("find_edge %p %p\n", cur_edge1, cur_edge2);
	}
	if (cur_edge1) {
		if (weight < cur_edge1->weight) {
			printf("found edge with weight %d while trying to apply weight %d\n", cur_edge1->weight, weight);
			cur_edge1->weight = weight;
		}
	}
	if (cur_edge2) {
		if (weight < cur_edge2->weight) {
			printf("found edge with weight %d while trying to apply weight %d\n", cur_edge2->weight, weight);
			cur_edge2->weight = weight;
		}
	}
	if (cur_edge2 || cur_edge1) return;
	//if (node1->type != '.' || node2->type != '.')
	//printf("connect %c %c (%p %p)\n", node1->type, node2->type, node1, node2);
	struct edge * new_edge = malloc(sizeof(struct edge));
	new_edge->weight = weight;
	new_edge->node1 = node1;
	new_edge->node2 = node2;

	struct edge_list * new_edge_list1 = malloc(sizeof(struct edge_list));
	new_edge_list1->edge = new_edge;
	new_edge_list1->prev = NULL;
	new_edge_list1->next = node1->first_edge;
	if (node1->first_edge) {
		node1->first_edge->prev = new_edge_list1;
	}
	node1->first_edge = new_edge_list1;

	struct edge_list * new_edge_list2 = malloc(sizeof(struct edge_list));
	new_edge_list2->edge = new_edge;
	new_edge_list2->prev = NULL;
	new_edge_list2->next = node2->first_edge;
	if (node2->first_edge) {
		node2->first_edge->prev = new_edge_list2;
	}
	node2->first_edge = new_edge_list2;
}

int match_edge(struct edge * e1, struct edge * e2)
{
	if (e1 == e2)
		return 1;
	if (e2->node1 == e1->node1 && e2->node2 == e1->node2)
		return 1;
	if (e2->node1 == e1->node2 && e2->node2 == e1->node1)
		return 1;

	return 0;
}

void remove_edge(struct edge_list * e, struct node * node)
{
	struct edge_list * next = e->next;
	struct edge_list * prev = e->prev;
	if (prev) {
		prev->next = next;
	} else {
		node->first_edge = next;
	}
	if (next) {
		next->prev = prev;
	}
	// now find the other end of this edge_list items edge, the one not connected to node
	struct node * other_node = e->edge->node1;
	if (node == other_node) {
		other_node = e->edge->node2;
	}
	if (node->type == 'S')
	printf("remove_edge: other_node: %p\n", other_node );
	// now find the edge in the other nodes edge_list
	struct edge_list * e2 = other_node->first_edge;
	if (node->type == 'S')
	printf("e2: %p \n", e2);
	while (e2) {
		if (match_edge(e2->edge, e->edge)) {
			next = e2->next;
			prev = e2->prev;
			if (prev) {
				prev->next = next;
			} else {
				other_node->first_edge = next;
			}
			if (next) {
				next->prev = prev;
			}
			break;
		}		e2 = e2->next;
	}
}

void eliminate_node(struct node * node)
{
//	printf("eliminate %c\n", node->type);
	struct edge_list * e = node->first_edge;
	// for each pair of edges in node, replace with one
	while (e) {
		struct edge_list * e2 = node->first_edge;
		struct edge_list * next = e->next;
		while (e2) {
			if (e->edge == e2->edge) {
				e2 = e2->next;
				continue;
			}
			// find the nodes to connect.
			struct node * n1 = e->edge->node1;
			if (n1 == node) {
				n1 = e->edge->node2;
			}
			struct node * n2 = e2->edge->node1;
			if (n2 == node) {
				n2 = e2->edge->node2;
			}
			// make the new connection
			connect_nodes(n1, n2, e->edge->weight + e2->edge->weight);
			// remove e and e2
			struct edge_list * next2 = e2->next;
			//remove_edge(e2, node);

			e2 = next2;
		}
		remove_edge(e, node);

		e = next;
	}
	struct node * next = node->next;
	struct node * prev = node->prev;
	if (prev) {
		prev->next = next;
	} else {
		first_node = next;
	}
	if (next) {
		next->prev = prev;
	}
}

struct node * new_node(char type, struct node * conn, int weight)
{
	struct node * new_node = malloc(sizeof(struct node));
	new_node->type = type;
	new_node->next = first_node;
	new_node->prev = NULL;
	new_node->first_edge = NULL;
	first_node->prev = new_node;
	first_node = new_node;
	connect_nodes(new_node, conn, weight);

	return new_node;
}


// origin is the point to connect the node to.
void parse_node(struct node * origin_node, int origin_pos, int dir)
{
	int this_pos = origin_pos + dir;

	char type = input[this_pos];
	if (type == '#' || type == '%') {
		return;
	}


	struct node * this_node = new_node(type, origin_node, 1);
	if (this_pos == 91) {
		node91 = this_node;
		//printf("node 91 is %p\n", node91);
	}

	input[this_pos] = '%';
	parse_node(this_node, this_pos, UP);
	parse_node(this_node, this_pos, DOWN);
	parse_node(this_node, this_pos, LEFT);
	parse_node(this_node, this_pos, RIGHT);
}

struct node * find_node(struct node * root_node, char type)
{
	struct node * n = root_node;
	while (n) {
		if (n->type == type)
			return n;
		n = n->next;
	}
	return NULL;
}

struct edge_list * copy_edge_list(struct edge_list * src, struct node * dst_first_node)
{
	struct edge_list * current_el = src;
	struct edge_list * new_first_el = NULL;
	struct edge_list * new_el_prev = NULL;
	while (current_el) {
		struct edge_list * new_el = malloc(sizeof(struct edge_list));
		if (!new_first_el) {
			new_first_el = new_el;
		}
		if (new_el_prev) {
			new_el_prev->next = new_el;
		}
		new_el->prev = new_el_prev;
		new_el->next = NULL;      // assigned when allocated, in next loop iteration

		new_el->edge = malloc(sizeof(struct edge));
		new_el->edge->weight = current_el->edge->weight;
		new_el->edge->node1 = find_node(dst_first_node, current_el->edge->node1->type);
		new_el->edge->node2 = find_node(dst_first_node, current_el->edge->node2->type);
		if (!new_el->edge->node1) {
			printf("cant find %c (1) in copy_edge_list\n", current_el->edge->node1->type);
			printf("copy\n");
			print_nodes(dst_first_node);
			printf("first_node\n");
			print_nodes(first_node);
		}
		if (!new_el->edge->node2) {
			printf("cant find %c (2) in copy_edge_list\n", current_el->edge->node2->type);
			printf("copy\n");
			print_nodes(dst_first_node);
			printf("first_node\n");
			print_nodes(first_node);
		}

		new_el_prev = new_el;
		current_el = current_el->next;
	}
	return new_first_el;
}

void dealloc_state(struct node * n)
{
}

struct node * save_state()
{
	struct node * current_node = first_node;
	struct node * new_first_node = NULL;
	struct node * new_node_prev = NULL;
	while (current_node) {
		struct node * new_node = malloc(sizeof(struct node));
		if (!new_first_node) {
			new_first_node = new_node;
		}
		if (new_node_prev) {
			new_node_prev->next = new_node;
		}
		new_node->type = current_node->type;
		new_node->prev = new_node_prev;
		new_node->next = NULL;       // assigned when allocated, in next loop iteration
		new_node->first_edge = NULL; // assign this in next step

		new_node_prev = new_node;
		current_node = current_node->next;
	}
	current_node = first_node;
	struct node * dst_node = new_first_node;
	while (current_node) {
		dst_node->first_edge = copy_edge_list(current_node->first_edge, new_first_node);

		current_node = current_node->next;
		dst_node = dst_node->next;
	}
	return new_first_node;
}

void compare_states(struct node * n1, struct node * n2)
{
	printf("compare...");
	struct node * t1 = n1;
	struct node * t2 = n2;
	while (t1) {
		if (t1->type != t2->type)
			printf("type differ\n");
		struct edge_list * el1 = t1->first_edge;
		struct edge_list * el2 = t2->first_edge;
		while (el1) {
			if (el1->edge->weight != el2->edge->weight)
				printf("weight differs\n");
			if (el1->edge->node1->type != el2->edge->node1->type)
				printf("node1 differs\n");
			if (el1->edge->node2->type != el2->edge->node2->type)
				printf("node2 differs\n");
			el1 = el1->next;
			el2 = el2->next;
		}
		if (el2) printf("t2 is longer than t1\n");
		t1 = t1->next;
		t2 = t2->next;
	}
	if (t2) printf("t2 is longer than t1\n");
	printf(" done!\n");
}

int best_score = -1;
int tot_runs = 0;
int recursion_level = 0;
int highest_recursion_level = 0;
int levels_above_100 = 0;
struct stack previous_steps;
int tot_visits = 0;
void try_keys(struct node * current, int steps)
{
	tot_visits++;
	if (tot_visits % 1000000 == 0) {
		printf("Visits %d ", tot_visits);
		struct stack ts = { .top = 0};
		while (previous_steps.top) {
			PUSH(ts, POP(previous_steps));
		}
		while (ts.top) {
			char tc = (char) POP(ts);
			PUSH(previous_steps, tc);

			printf("%c ", (char) tc);
		}
		printf("%c\n", current->type);
	}

	if (best_score > -1) {
		if (steps > best_score) return;
	}

	/*
	recursion_level++;



	if (recursion_level < 27) {
	//	printf("recursion level: %d\n", recursion_level);
	}
	if (recursion_level > 26) {
		if (recursion_level > highest_recursion_level) {
			highest_recursion_level = recursion_level;
		}
//		printf("recursion level %d tot_runs:%d\n", recursion_level, tot_runs);
//		print_nodes(first_node);
		levels_above_100++;
		//if (levels_above_100 > 5) { exit(0); }
		if (recursion_level > 30) {
			exit(0);
		}
	}
	*/

	struct node * copy_of_state = save_state();

	struct fifo available_keys = { 0, 0 };
	struct fifo steps_to_keys = { 0, 0 };
	struct edge_list * e = current->first_edge;
	while (e) { // find available keys that are not the current
		int64_t key;
		key = e->edge->node1->type;
		if (key >= 'a' && key != current->type) {
			PUT(available_keys, key);
			PUT(steps_to_keys, e->edge->weight);
		}
		key = e->edge->node2->type;
		if (key >= 'a' && key != current->type) {
			PUT(available_keys, key);
			PUT(steps_to_keys, e->edge->weight);
		}
		e = e->next;
	}
	if (EMPTY(available_keys)) { // end condition
		tot_runs++;
		if (tot_runs % 1000000 == 0) {
			printf("tot runs: %d\n", tot_runs);
		}

		if (best_score == -1) {
			best_score = steps;
		} else if (steps < best_score) {
			printf("Finished! Runs:%d Steps:%d\n", tot_runs, steps);
			best_score = steps;
			struct stack ts = { .top = 0};
			while (previous_steps.top) {
				PUSH(ts, POP(previous_steps));
			}
			while (ts.top) {
				char tc = (char) POP(ts);
				PUSH(previous_steps, tc);

				printf("%c ", (char) tc);
			}
			printf("%c\n", current->type);
		}
		first_node = copy_of_state;
		recursion_level--;
		return;
	}

	eliminate_node(current);
	struct node * t = save_state();
	while (!EMPTY(available_keys)) {
		char k = (char)GET(available_keys);
		int s = (int)GET(steps_to_keys);
		if (steps + s > best_score && best_score > -1) continue;
		t = save_state();
		eliminate_node(find_node(first_node, k - 0x20)); // remove the locked door for next iteration
		//printf("try %c %d\n", k, available_keys.write - available_keys.read);
		PUSH(previous_steps, (int) current->type);
		try_keys(find_node(first_node, k), steps + s);
		(void)POP(previous_steps);
		first_node = t;
		t = save_state();

		//printf("returned %c\n", k);

		//printf("%c", (char)GET(available_keys));
	}
	first_node = copy_of_state;
	recursion_level--;
}

int main(int argc, char * argv[])
{
	(void) read(STDIN_FILENO, input, sizeof(input));

	int i = 0;
	while (input[i] != '@') {
		i++;
	}

	input[i] = '%';
	first_node = malloc(sizeof(struct node));
	first_node->next = NULL,
	first_node->prev = NULL,
	first_node->first_edge = NULL,
	first_node->type = '@',
	parse_node(first_node, i, UP);
	parse_node(first_node, i, DOWN);
	parse_node(first_node, i, LEFT);
	parse_node(first_node, i, RIGHT);

	struct node * n = first_node;

	int dots = 1;
	while (dots > 0) {
		n = first_node;
		i = 0;
		dots = 0;
		while (n) {
			struct node * next = n->next;
			if (n->type == '.') {
				//print_edges(n, 1);
				eliminate_node(n);
				dots++;
			}
			i++;
			n = next;
		}
		printf("removed %d nodes out of %d\n", dots, i);
	}

	n = first_node;
	i = 0;
	while (n) {
		struct node * next = n->next;
		print_edges(n, 1);
		i++;
		n = next;
	}

	//int64_t comb[27] = {
	//	'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i',
	//	'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r',
	//	's', 't', 'u', 'v', 'w', 'x', 'y', 'z', -1 };
	//struct stack s = { 0, { 0 }};
	//permute(comb, 26, &s);

	save_state();
	try_keys(find_node(first_node, '@'), 0);
	// find the set of available keys.
	//  for each available key
	//  PUSH state of graph
	//   try each key k:
	//    POP state of graph
	//    unlock door K
	//     if no keys are available from new starting point k we're done
	//     ** otherwise recurse
	//
	return 0;
}

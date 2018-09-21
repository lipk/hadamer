#include <tree.h>
#include <stdio.h>

void test_sampling() {
	Tree *tree = Tree_create();
	for (int y = 0; y<BLOCKSIZE; ++y) {
		for (int x = 0; x<BLOCKSIZE; ++x) {
			*GET(double, tree->root->data[0], x, y) = 1.0;
		}
	}
	tree->root->action = REFINE;
	Tree_restructure(tree);
	for (int i = 0; i<TREECH; ++i) {
		Node *node = tree->root->children[i];
		for (int y = 0; y<BLOCKSIZE; ++y) {
			for (int x = 0; x<BLOCKSIZE; ++x) {
				assert(*GET(double, node->data[0], x, y) == 0.25);
			}
		}
		node->action = DEREFINE;
	}
	Tree_restructure(tree);
	for (int y = 0; y<BLOCKSIZE; ++y) {
		for (int x = 0; x<BLOCKSIZE; ++x) {
			assert(*GET(double, tree->root->data[0], x, y) == 1.0);
		}
	}
}

void test_basic() {
	Tree *tree = Tree_create();
	assert(tree->root->data != NULL);
	tree->root->action = REFINE;
	Tree_restructure(tree);
	assert(tree->root->data[0] == NULL);
	for (int i = 0; i<TREECH; ++i) {
		assert(tree->root->children[i]->data[0] != NULL);
		tree->root->children[i]->action = DEREFINE;
	}
	Tree_restructure(tree);
	assert(tree->root->data[0] != NULL);
}

int main() {
	test_basic();
	test_sampling();
	return 0;
}

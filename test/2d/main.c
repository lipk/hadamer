#include <tree.h>
#include <stdio.h>
#include <refplan.h>
#include <util.h>

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

void test_merge_plan() {
	RefinePlan plan1;
	plan1.level = 3;
	plan1.pos[0] = 0;
	plan1.pos[1] = 0;
	plan1.size[0] = 1;
	plan1.size[1] = 1;

	RefinePlan plan2;
	plan2.level = 3;
	plan2.pos[0] = 0;
	plan2.pos[1] = 1;
	plan2.size[0] = 1;
	plan2.size[1] = 1;

	{
		RefinePlan *merged = RefinePlan_mergeIfPossible(&plan1, 0, &plan1, 1, 2);
		assert(merged != NULL);
		assert(merged->level == 3);
		assert(merged->pos[0] == 0);
		assert(merged->pos[1] == 0);
		assert(merged->size[0] == 2);
		assert(merged->size[1] == 1);
		DESTROY(RefinePlan, merged);
	}

	{
		RefinePlan *merged = RefinePlan_mergeIfPossible(&plan1, 0, &plan1, 2, 2);
		assert(merged != NULL);
		assert(merged->level == 3);
		assert(merged->pos[0] == 0);
		assert(merged->pos[1] == 0);
		assert(merged->size[0] == 1);
		assert(merged->size[1] == 2);
		DESTROY(RefinePlan, merged);
	}

	{
		RefinePlan *merged = RefinePlan_mergeIfPossible(&plan1, 0, &plan1, 3, 2);
		assert(merged == NULL);
	}

	{
		RefinePlan *merged = RefinePlan_mergeIfPossible(&plan1, 1, &plan1, 0, 2);
		assert(merged != NULL);
		assert(merged->level == 3);
		assert(merged->pos[0] == 0);
		assert(merged->pos[1] == 0);
		assert(merged->size[0] == 2);
		assert(merged->size[1] == 1);
		DESTROY(RefinePlan, merged);
	}

	{
		RefinePlan *merged = RefinePlan_mergeIfPossible(&plan1, 1, &plan1, 3, 2);
		assert(merged != NULL);
		assert(merged->level == 3);
		assert(merged->pos[0] == 1);
		assert(merged->pos[1] == 0);
		assert(merged->size[0] == 1);
		assert(merged->size[1] == 2);
		DESTROY(RefinePlan, merged);
	}

	{
		RefinePlan *merged = RefinePlan_mergeIfPossible(&plan1, 1, &plan1, 2, 2);
		assert(merged == NULL);
	}

	{
		RefinePlan *merged = RefinePlan_mergeIfPossible(&plan2, 1, &plan1, 3, 1);
		assert(merged != NULL);
		assert(merged->level == 3);
		assert(merged->pos[0] == 2);
		assert(merged->pos[1] == 1);
		assert(merged->size[0] == 1);
		assert(merged->size[1] == 2);
		DESTROY(RefinePlan, merged);
	}
}

int main() {
	test_basic();
	test_sampling();
	test_merge_plan();
	return 0;
}

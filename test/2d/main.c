#include <tree.h>
#include <stdio.h>
#include <refplan.h>
#include <util.h>

void test_sampling() {
    Tree *tree = Tree_create();
    for (int y = 0; y<BLOCKSIZE; ++y) {
        for (int x = 0; x<BLOCKSIZE; ++x) {
            *GET(double, tree->root->data[0].source, x+1, y+1) = 1.0;
        }
    }
    tree->root->action = REFINE;
    Tree_restructure(tree);
    for (int i = 0; i<TREECH; ++i) {
        Node *node = tree->root->children[i];
        for (int y = 0; y<BLOCKSIZE; ++y) {
            for (int x = 0; x<BLOCKSIZE; ++x) {
                assert(*GET(double, node->data[0].source, x+1, y+1) == 0.25);
            }
        }
        node->action = DEREFINE;
    }
    Tree_restructure(tree);
    for (int y = 0; y<BLOCKSIZE; ++y) {
        for (int x = 0; x<BLOCKSIZE; ++x) {
            assert(*GET(double, tree->root->data[0].source, x+1, y+1) == 1.0);
        }
    }
}

void test_basic() {
    Tree *tree = Tree_create();
    Tree_export(tree, "basic1");
    assert(tree->root->data != NULL);
    tree->root->action = REFINE;
    Tree_restructure(tree);
    Tree_export(tree, "basic2");
    assert(tree->root->children[0] != NULL);
    assert(*GET(Node*, tree->root->children[0]->adjacent, 0, 0) == NULL);
    assert(*GET(Node*, tree->root->children[0]->adjacent, 1, 0) == NULL);
    assert(*GET(Node*, tree->root->children[0]->adjacent, 2, 0) == NULL);
    assert(*GET(Node*, tree->root->children[0]->adjacent, 0, 1) == NULL);
    assert(*GET(Node*, tree->root->children[0]->adjacent, 1, 1) == tree->root->children[0]);
    assert(*GET(Node*, tree->root->children[0]->adjacent, 2, 1) == tree->root->children[1]);
    assert(*GET(Node*, tree->root->children[0]->adjacent, 0, 2) == NULL);
    assert(*GET(Node*, tree->root->children[0]->adjacent, 1, 2) == tree->root->children[2]);
    assert(*GET(Node*, tree->root->children[0]->adjacent, 2, 2) == tree->root->children[3]);
    assert(tree->root->children[1] != NULL);
    assert(*GET(Node*, tree->root->children[1]->adjacent, 0, 0) == NULL);
    assert(*GET(Node*, tree->root->children[1]->adjacent, 1, 0) == NULL);
    assert(*GET(Node*, tree->root->children[1]->adjacent, 2, 0) == NULL);
    assert(*GET(Node*, tree->root->children[1]->adjacent, 0, 1) == tree->root->children[0]);
    assert(*GET(Node*, tree->root->children[1]->adjacent, 1, 1) == tree->root->children[1]);
    assert(*GET(Node*, tree->root->children[1]->adjacent, 2, 1) == NULL);
    assert(*GET(Node*, tree->root->children[1]->adjacent, 0, 2) == tree->root->children[2]);
    assert(*GET(Node*, tree->root->children[1]->adjacent, 1, 2) == tree->root->children[3]);
    assert(*GET(Node*, tree->root->children[1]->adjacent, 2, 2) == NULL);
    for (int i = 0; i<TREECH; ++i) {
        assert(tree->root->children[i]->data[0].source != NULL);
        tree->root->children[i]->action = DEREFINE;
    }
    Tree_restructure(tree);
    Tree_export(tree, "basic3");
    assert(tree->root->children[0] == NULL);
}

void init_refplan1(Buffer *buf, RefinePlan *plan) {
    uint64_t npos[2] = {0, 0};
    uint64_t nsize[2] = {1, 1};
    plan->propagate = true;
    plan->level = 3;
    plan->pos[0] = 0;
    plan->pos[1] = 0;
    plan->size[0] = 1;
    plan->size[1] = 1;
    plan->nodes = Array_create(buf, nsize, npos);
}

void init_refplan2(Buffer *buf, RefinePlan *plan) {
    uint64_t npos[2] = {0, 0};
    uint64_t nsize[2] = {1, 1};
    plan->propagate = true;
    plan->level = 3;
    plan->pos[0] = 0;
    plan->pos[1] = 1;
    plan->size[0] = 1;
    plan->size[1] = 1;
    plan->nodes = Array_create(buf, nsize, npos);
}

void test_merge_plan_simple() {
    uint64_t bufsize[2] = {2, 1};
    Buffer *buf = Buffer_create(8, 8, bufsize);
    {
        RefinePlan plan1, plan2;
        init_refplan1(buf, &plan1);
        init_refplan1(buf, &plan2);
        RefinePlan_propagate(&plan1, 0, 2);
        RefinePlan_propagate(&plan2, 1, 2);
        RefinePlan *merged = RefinePlan_mergeIfPossible(&plan1, &plan2, 2);
        assert(merged != NULL);
        assert(merged->level == 3);
        assert(merged->pos[0] == 0);
        assert(merged->pos[1] == 0);
        assert(merged->size[0] == 2);
        assert(merged->size[1] == 1);
        DESTROY(RefinePlan, merged);
    }

    {
        RefinePlan plan1, plan2;
        init_refplan1(buf, &plan1);
        init_refplan1(buf, &plan2);
        RefinePlan_propagate(&plan1, 0, 2);
        RefinePlan_propagate(&plan2, 2, 2);
        RefinePlan *merged = RefinePlan_mergeIfPossible(&plan1, &plan2, 2);
        assert(merged != NULL);
        assert(merged->level == 3);
        assert(merged->pos[0] == 0);
        assert(merged->pos[1] == 0);
        assert(merged->size[0] == 1);
        assert(merged->size[1] == 2);
        DESTROY(RefinePlan, merged);
    }

    {
        RefinePlan plan1, plan2;
        init_refplan1(buf, &plan1);
        init_refplan1(buf, &plan2);
        RefinePlan_propagate(&plan1, 0, 2);
        RefinePlan_propagate(&plan2, 3, 2);
        RefinePlan *merged = RefinePlan_mergeIfPossible(&plan1, &plan2, 2);
        assert(merged == NULL);
    }

    {
        RefinePlan plan1, plan2;
        init_refplan1(buf, &plan1);
        init_refplan1(buf, &plan2);
        RefinePlan_propagate(&plan1, 1, 2);
        RefinePlan_propagate(&plan2, 0, 2);
        RefinePlan *merged = RefinePlan_mergeIfPossible(&plan1, &plan2, 2);
        assert(merged != NULL);
        assert(merged->level == 3);
        assert(merged->pos[0] == 0);
        assert(merged->pos[1] == 0);
        assert(merged->size[0] == 2);
        assert(merged->size[1] == 1);
        DESTROY(RefinePlan, merged);
    }

    {
        RefinePlan plan1, plan2;
        init_refplan1(buf, &plan1);
        init_refplan1(buf, &plan2);
        RefinePlan_propagate(&plan1, 1, 2);
        RefinePlan_propagate(&plan2, 3, 2);
        RefinePlan *merged = RefinePlan_mergeIfPossible(&plan1, &plan2, 2);
        assert(merged != NULL);
        assert(merged->level == 3);
        assert(merged->pos[0] == 1);
        assert(merged->pos[1] == 0);
        assert(merged->size[0] == 1);
        assert(merged->size[1] == 2);
        DESTROY(RefinePlan, merged);
    }

    {
        RefinePlan plan1, plan2;
        init_refplan1(buf, &plan1);
        init_refplan1(buf, &plan2);
        RefinePlan_propagate(&plan1, 1, 2);
        RefinePlan_propagate(&plan2, 2, 2);
        RefinePlan *merged = RefinePlan_mergeIfPossible(&plan1, &plan2, 2);
        assert(merged == NULL);
    }

    {
        RefinePlan plan1, plan2;
        init_refplan2(buf, &plan1);
        init_refplan1(buf, &plan2);
        RefinePlan_propagate(&plan1, 1, 1);
        RefinePlan_propagate(&plan2, 3, 1);
        RefinePlan *merged = RefinePlan_mergeIfPossible(&plan1, &plan2, 1);
        assert(merged != NULL);
        assert(merged->level == 3);
        assert(merged->pos[0] == 2);
        assert(merged->pos[1] == 1);
        assert(merged->size[0] == 1);
        assert(merged->size[1] == 2);
        DESTROY(RefinePlan, merged);
    }

    {
        RefinePlan plan1, plan2;
        init_refplan1(buf, &plan1);
        init_refplan2(buf, &plan2);
        RefinePlan_propagate(&plan1, 1, 1);
        RefinePlan_propagate(&plan2, 3, 1);
        RefinePlan *merged = RefinePlan_mergeIfPossible(&plan1, &plan2, 1);
        assert(merged == NULL);
    }
}

void test_tall_tree()
{
    Tree *tree = Tree_create();
    tree->root->action = REFINE;
    Tree_restructure(tree);
    tree->root->children[0]->action = REFINE;
    tree->root->children[1]->action = REFINE;
    tree->root->children[2]->action = REFINE;
    Tree_restructure(tree);
    tree->root->children[2]->children[1]->action = REFINE;
    Tree_restructure(tree);
    Tree_export(tree, "tall_tree");
}

void test_kernel()
{
    Tree *tree = Tree_create();
    for (int i = 0; i<3; ++i) {
        Tree_mass_applyKernel(tree, &Kernel_blur);
    }
}

int main() {
    test_basic();
    test_sampling();
    test_merge_plan_simple();
    test_tall_tree();
    test_kernel();
    return 0;
}

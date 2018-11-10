#include <tree.h>
#include <stdio.h>
#include <refplan.h>
#include <util.h>

void test_kernel()
{
    Tree *tree = Tree_create();
    for (int i = 0; i<2; ++i) {
        Tree_mass_applyKernel(tree, &Kernel_blur);
    }
}

int main() {
    test_kernel();
    return 0;
}

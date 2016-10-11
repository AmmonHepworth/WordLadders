#include "avlTree.h"
#include <cstdlib>
#include <string>

int main()
{
	string blah;
	int y;
	AvlTree<int> tree;

	tree.insert(1);
	tree.insert(3);
	tree.insert(5);
	tree.insert(7);
	tree.insert(9);
	tree.insert(9);
	tree.insert(9);
	tree.insert(11);
	tree.insert(2);
	tree.insert(9);
	tree.insert(4);
	tree.insert(8);
	std::cout << tree.toString(blah) << std::endl;
	tree.remove(7);
	tree.remove(9);
	std::cout << tree.toString(blah) << std::endl;
	tree.insert(30);
	tree.insert(50);
	tree.insert(30);
	tree.insert(30);
	tree.insert(15);
	tree.insert(18);
	std::cout << tree.toString(blah) << std::endl;
	tree.removeMin();
	std::cout << tree.toString(blah) << std::endl;
	tree.removeMin();
	std::cout << tree.toString(blah) << std::endl;
	tree.removeMin();
	std::cout << tree.toString(blah) << std::endl;
	tree.insert(17);
	std::cout << tree.toString(blah) << std::endl;
}

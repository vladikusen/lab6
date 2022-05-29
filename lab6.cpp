#include <iostream>
#include <chrono>
#include "foundation.h"

int main()
{
	using ptrFoundation = std::shared_ptr<Foundation>;


	ptrFoundation root = std::make_shared<Folder>("root");
	ptrFoundation folder1 = std::make_shared<Folder>("folder1");
	ptrFoundation folder2 = std::make_shared<Folder>("folder2");
	ptrFoundation folder3 = std::make_shared<Folder>("folder3");
	ptrFoundation folder4 = std::make_shared<Folder>("folder4");
	ptrFoundation folder5 = std::make_shared<Folder>("folder5");
	ptrFoundation file1 = std::make_shared<File>("file1");
	ptrFoundation file2 = std::make_shared<File>("file2");
	ptrFoundation file3 = std::make_shared<File>("file3");
	ptrFoundation file4 = std::make_shared<File>("file4");
	ptrFoundation file5 = std::make_shared<File>("file5");

	folder3->createElement(file1);
	folder2->createElement(folder3);
	folder1->createElement(file3);
	folder1->createElement(folder4);
	folder4->createElement(file4);
	folder4->createElement(folder5);
	folder5->createElement(file5);

	root->createElement(folder1);
	root->createElement(folder2);
	root->createElement(file2);

	root->who();
	std::cout << "\nFILE TREE WAS CREATED.\n" << std::endl;
	std::cout << "\n";
	//Let's find a folder/file by its path
	ptrFoundation byPathFile1 = root->getByPath(folder5->getPath());
	std::cout << "Found file by path:\n"; byPathFile1->print();
	//Let's find a folder/file by its name
	ptrFoundation byNameFile1 = root->getByName("file1");
	std::cout << "Found file by name:\n"; byNameFile1->print();
	std::cout << "\n";

	std::cout << "\n";
	//We are able to delete element as soon as we know its address
	std::cout << "After removal" << std::endl;
	byNameFile1->deleteElement();
	std::cout << "\n";
	root->who();
	
	//We can move a folder/file to another folder
	folder1->move(folder2);
	std::cout << "After movement" << std::endl;
	root->who();
	//We can copy a folder/file to another folder
	std::cout << "After copying" << std::endl;
	folder5->copy(folder2, "cloned");

	root->who();

	//root->print();

	std::cout << std::endl << std::endl;
	//Searching by mask and receiving a multiset of matching elements
	auto container = root->getByMask("(file[^ ]*)");

	for (const auto& file : container) {
		std::cout << file->getName() << std::endl;
	}


	

}



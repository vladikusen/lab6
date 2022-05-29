#ifndef __FOUNDATION_H__
#define __FOUNDATION_H__

#include <string>
#include <chrono>
#include <ctime>
#include "date.h"
#include <iostream>
#include <memory>
#include <list>
#include <set>
#include <regex>

class Foundation;

using spF = std::shared_ptr<Foundation>;
using lspF = std::list<spF>;

template<typename U, typename T>
class Iterator {
public:
	typedef typename std::list<T>::iterator it_t;
	Iterator(U& const _children) : children(_children) {
		it = children.begin();
	}
	void first() {
		it = children.begin();
	}
	void next() {
		it++;
	}
	auto current() {
		return *it;
	}
	bool isDone() const {
		return (it == children.end());
	}
private:
	U& children;
	it_t it;
};

class Foundation : public std::enable_shared_from_this<Foundation>
{
protected:
	std::string fileName {};
	spF parent;
	Date creationDate;
	Date modificationDate;

	void recalculateModificationTime(Date& date);
	virtual bool remove(spF) { return true; };
public:
	Foundation(const std::string& _name) : fileName(_name) {
		
		recalculateModificationTime(modificationDate);
		creationDate = modificationDate;

	}

	Foundation(const Foundation& that) = default;

	virtual std::size_t calculateSize() { return sizeof(Foundation); }

	virtual void print() {
		std::cout << "i am a foundation." << std::endl;
	}

	void setParent(spF _parent) {
		parent = std::move(_parent);
		recalculateModificationTime(modificationDate);
	}
	void setName(std::string _name) {
		fileName = _name;
		recalculateModificationTime(modificationDate);
	}
	
	auto getName() const { return fileName; }
	auto getParent() const { return parent; }
	auto getPath() const {
		auto _current = shared_from_this();
		std::string path = "";
		while (_current != nullptr) {
			path = "/" + _current->getName() + path;
			
			_current = _current->getParent();
		}

		return path;
	};
	auto getRoot() {
		auto buffer = shared_from_this();
		while(buffer->getParent() != nullptr) {
			buffer = buffer->getParent();
		}

		return buffer ;
	}
	virtual spF getByPath(std::string) { return nullptr; }
	virtual spF getByName(std::string) { return nullptr; }
	virtual std::multiset<spF> getByMask(std::string) { return std::multiset<spF>(); }
	auto getCreationDate() const {
		return creationDate.getDate();
	}
	auto getCreationTime() const {
		return creationDate.getTime();
	}

	virtual void deleteElement() {
		getParent()->remove(shared_from_this());
	}

	virtual void move(spF destination) {
		deleteElement();
		destination->createElement(shared_from_this());
		recalculateModificationTime(modificationDate);
	}
	virtual void copy(spF destination, std::string newName = "") {
	}

	virtual std::unique_ptr<Iterator<lspF, spF>> createIterator() {
		return nullptr;
	};
	
	virtual bool isFolder() const { return false;  }

	
	virtual bool createElement(spF) { return true; };
	virtual std::size_t getSubElements() const { return 0; }

	virtual spF clone() {
		return std::make_shared<Foundation>(*this);
	}

	virtual void who()  { std::cout << fileName; }

	virtual void clear() {}

	virtual void getAllNames(std::string& result) const { result += fileName + " "; }

};

class File : public Foundation {
protected:
	std::size_t calculateSize() override {
		
		return sizeof(File);
	}
public:
	File(std::string&& name) : Foundation(std::move(name))
	{}
	File(const std::string& name) : Foundation(name) {}

	File(const File& that) : Foundation(that.fileName) {}
	void who() override { 
		auto count = [](std::string str) {
			int counter = 0;
			for (const auto& character : str) {
				if (character == '/') {
					counter++;
				}
			}
			return counter;
		};
		std::string prep = "";
		for (int i = 1; i < count(getPath()); i++) {
			prep += "\t";
		}
		std::cout << prep << "i am a file " << fileName << "(" << getPath() << ")" << " " << getCreationDate() << " " << getCreationTime() << " Size=" << calculateSize() << std::endl;
	}
	spF getByPath(std::string path) override {
		if (getPath() == path)
		{
			return shared_from_this();
		}
		else {
			return nullptr;
		}
	}

	void print() override {
		std::cout << "i am a file." << fileName << std::endl;
	}

	spF clone() override {
		return std::make_shared<File>(*this);
	}

	spF getByName(std::string _name) {
		if (_name == fileName) {
			return shared_from_this();
		}
		else {
			return nullptr;
		}
	}

	void copy(spF destination, std::string newName = "") override {
		auto cloned = clone();
		if (newName != "") {
			cloned->setName(newName);
		}
		destination->createElement(cloned);
	}

};


class Folder : public Foundation {
	friend class Iterator<lspF, spF>;
private:
	lspF subF;
protected:
	std::size_t calculateSize() override {

		return subF.size() * sizeof(Foundation) + sizeof(Foundation);
	}

	bool remove(spF element) override {
		subF.remove(element);
		return true; 
	}

public:

	Folder(std::string&& name) : Foundation(std::move(name)) {}
	
	Folder(const std::string& name) : Foundation(name) {}
	
	std::size_t getSubElements() const { return subF.size(); }

	bool createElement(spF) override;

	void getAllNames(std::string& result) const override {
		result += fileName + " ";
		for (const auto& files : subF) {
			files->getAllNames(result);
		}
	}
	

	void who() override {
		auto count = [](std::string str) {
			int counter = 0;
			for (const auto& character : str) {
				if (character == '/') {
					counter++;
				}
			}
			return counter;
		};
		std::string prep = "";
		for (int i = 1; i < count(getPath()); i++) {
			prep += "\t";
		}
		std::cout  << prep << "I am a folder " << fileName << "(" << getPath() << ")" << " " << getCreationDate() << " " << getCreationTime() << " Size=" << calculateSize() << std::endl;
		for (const auto& file : subF) {
			file->who();
		}
	}
	bool isFolder() const override { return true; }

	spF getByPath(std::string path) override {
		if (path == getPath()) {
			return shared_from_this();
		}
		for (const auto& files : subF) {
			if (files->getByPath(path) != nullptr) {
				return files->getByPath(path);
			}
		}
		return nullptr;
	}

	spF getByName(std::string _name) override {
		if (fileName == _name) {
			return shared_from_this();
		}
		for (const auto& files : subF) {
			if (files->getByName(_name) != nullptr) {
				return files->getByName(_name);
			}
		}
		return nullptr;
	}

	std::multiset<spF> getByMask(std::string mask) override {
		std::regex reg(mask);
		auto allNames = std::string("");
		getRoot()->getAllNames(allNames);

		std::sregex_iterator firstMatch(allNames.begin(), allNames.end(), reg);
		std::sregex_iterator lastMatch;
		
		std::multiset<spF> matches;

		while (firstMatch != lastMatch) {
			std::smatch match = *firstMatch;
			if(getByName(match.str()) != nullptr) {
				matches.insert(getRoot()->getByName(match.str()));
			}
			firstMatch++;
		}

		return matches;
	}

	void print() override {
		std::cout << "i am a file." << fileName << std::endl;
		for (const auto& files : subF) {
			files->print();
		}
	}

	spF clone() override {
		return std::make_shared<Folder>(*this);
	}

	std::unique_ptr<Iterator<lspF, spF>> createIterator() override {
		return std::make_unique<Iterator<lspF, spF>>(subF);
	}

	void clear() override {
		subF.clear();
	}

	void copy(spF destination, std::string newName = "") override {
		auto created = clone();
		if (newName != "") {
			created->setName(newName);
		}
		created->clear();
		for (const auto& file : subF) {
			file->copy(created);
		}
		
		destination->createElement(std::move(created));
		
	}

};

#endif __FOUNDATION_H__ 


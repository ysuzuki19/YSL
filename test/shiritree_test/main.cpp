#include <iostream>
#include "../../include/ShiriTree.hpp"
using namespace std;

int main(int argc, char *argv[]){
	wcout.imbue(std::locale("en_US.UTF-8"));
	std::string filename;
	if(argc == 2){
		filename = argv[1];
		std::cout << "[LOG] filename to ShiriTree : " << filename << std::endl;
	}else{
		std::cout << "[ERROR] Please input argv[1] : filename to ShiriTree." << std::endl;
		return 0;
	}
	YSL::ShiriTree shiritori;
	shiritori.load(filename);
	cout << "Loaded Words List " << endl;
	//shiritori.setLogging(false);
	shiritori.view_words();
	shiritori.solve();
	cout << "Loaded     Words Length : " << shiritori.wordSize() << endl;
	cout << "Continuous Words Length : " << shiritori.length() << endl;
	cout << "Words Usage Rate        : " << shiritori.usageRate() << endl;
	shiritori.view_continuous_words();
	shiritori.report("result.txt");
	return 0;
}


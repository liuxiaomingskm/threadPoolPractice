#pragma once

#define LOG(str) \
	cout << __FILE__ << ":" << __LINE__ << " " << \
	__DATE__ << " " <<__TIME__ << " : " << str << endl;
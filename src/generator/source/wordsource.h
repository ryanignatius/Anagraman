#pragma once

class wordsource {
	public:
	virtual size_t size() = 0;
	virtual std::string get_one(int) = 0;
	virtual bool is_existed(std::string) = 0;
};
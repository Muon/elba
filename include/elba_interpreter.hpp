#ifndef ELBA_INTERPRETER_H
#define ELBA_INTERPRETER_H

struct lua_State;

namespace elba
{

class interpreter
{
public:
	interpreter();
	~interpreter();

	void openLibs();
	
	void doString(const char* str);
private:
	lua_State* state;
};

}

#endif
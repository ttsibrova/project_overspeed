#pragma once
#include <Object/IObject.h>

class Command
{
public:
    virtual ~Command() {}
    virtual bool Execute (IObject* obj) = 0;
};

//class CommandWithContext: public Command
//{
//public:
//    CommandWithContext (IObject* contextObj):
//        m_contextObj (contextObj)
//    {}
//
//    bool ExecuteContext() { return Execute (m_contextObj); }
//
//private:
//    IObject* m_contextObj;
//};

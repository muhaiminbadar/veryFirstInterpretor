#pragma once
#include <string>
#include <stack>

enum EquationType {
    NONE,
    POSTFIX,
    INFIX,
    PREFIX
};

namespace Utility 
{
    static const std::string operators = "*+/-";
    bool isOperator(const char& x) 
    { 
        if(operators.find(x) != -1)
        {
            return true; 
        }
        return false; 
    }
    std::string convert(const std::string& equation, EquationType et)
    {
        std::string returnStr;
        if(et == POSTFIX) // Prefix -> Postfix
        {
            std::stack<std::string> Stack;
            for(int i = equation.length() - 1; i >= 0; i--)
            {
                if(isOperator(equation[i]))
                {
                    string t1 = Stack.top();
                    Stack.pop();
                    string t2 = Stack.top();
                    Stack.pop();
                    Stack.emplace(t1 + t2 + equation[i]);
                }
                else
                {
                    Stack.push(string(1, equation[i]));
                }   
            }
            returnStr = Stack.top();
        }
        else if(et == PREFIX)// Postfix -> Prefix
        {
            std::stack<std::string> Stack;
            for(const char& c : equation)
            {
                if(isOperator(c))
                {
                    string t1 = Stack.top();
                    Stack.pop();
                    string t2 = Stack.top();
                    Stack.pop();
                    Stack.emplace(c + t2 + t1);
                }
                else
                {
                    Stack.push(string(1, c));
                }   
            }
            while (!Stack.empty()) {
                returnStr += Stack.top();
                Stack.pop();
            }
        }
        return returnStr;
    }
    std::string typeName(const EquationType& et)
    {
        std::string returnStr;
        if(et == POSTFIX)
        {
            returnStr = "postfix:";
        }
        else if(et == PREFIX)
        {
            returnStr = "prefix:";
        }
        return returnStr;
    }
}
#include <iostream>
#include <string>
#include <fstream>
#include <queue>

#include "ArgumentManager.h"
#include "Containers.h"
#include "Utility.h"

using namespace std;

struct InputLine {
    EquationType et;
    std::string equation;
};

std::string sanitizedInput(std::string& inputString, bool lower = false)
{
    std::string returnStr;
    for (char & c : inputString)
    {
        if (c != ' ')
        {
            returnStr += !lower ? c : std::tolower(static_cast<unsigned char>(c));
        }
    }
    return returnStr;
}

void ProcessCommands(LList<InputLine*>* inputs, fstream& cmdfs, fstream& output)
{
    int line = 0;
    std::string buffer;
    LListStack<InputLine*> reserveStack;
    std::queue<Node<InputLine*>*> deleteQueue;
    while(getline(cmdfs, buffer))
    {
        if(!buffer.empty())
        {
            buffer = sanitizedInput(buffer, true);
            size_t pos = buffer.find('(');
            std::string cmdName = buffer.substr(0, pos == -1 ? buffer.length() : pos);
            // --------------------------------------Convert List Command.
            if(cmdName == "convertlist")
            {
                std::string argument = buffer.substr(pos+1, buffer.find(')')-pos-1);
                if(argument == "postfix")
                {
                    for(auto* i = inputs->getHead(); i != nullptr; i = i->next)
                    {
                        if(i->data->et == POSTFIX)
                        {
                            i->data->equation = Utility::convert(i->data->equation, PREFIX);
                            i->data->et = PREFIX;
                        }
                    }
                }
                else if(argument == "prefix")
                {
                    for(auto* i = inputs->getHead(); i != nullptr; i = i->next)
                    {
                        if(i->data->et == PREFIX)
                        {
                            i->data->equation = Utility::convert(i->data->equation, POSTFIX);
                            i->data->et = POSTFIX;
                        }
                    }
                }
                else if(argument == "all")
                {
                    for(auto* i = inputs->getHead(); i != nullptr; i = i->next)
                    {
                        if(i->data->et == POSTFIX)
                        {
                            i->data->equation = Utility::convert(i->data->equation, PREFIX);
                            i->data->et = PREFIX;
                        }
                        else
                        {
                            i->data->equation = Utility::convert(i->data->equation, POSTFIX);
                            i->data->et = POSTFIX;
                        }
                        
                    }
                }
                else
                {
                    // Must be an integer here
                    int position = std::stoi(argument);
                    if(position < inputs->size())
                    {
                        if(position <= 0)
                        {
                            if(inputs->getHead() != nullptr)
                            {
                                if(inputs->getHead()->data->et == POSTFIX)
                                {
                                    inputs->getHead()->data->equation = Utility::convert(inputs->getHead()->data->equation, PREFIX);
                                    inputs->getHead()->data->et = PREFIX;
                                }
                                else
                                {
                                    inputs->getHead()->data->equation = Utility::convert(inputs->getHead()->data->equation, POSTFIX);
                                    inputs->getHead()->data->et = POSTFIX;
                                }
                            }
                        }
                        else
                        {
                            int count = 0;
                            for(auto* i = inputs->getHead(); i != nullptr; i = i->next, count++)
                            {
                                if(count == position)
                                {
                                    if(i->data->et == POSTFIX)
                                    {
                                        i->data->equation = Utility::convert(i->data->equation, PREFIX);
                                        i->data->et = PREFIX;
                                    }
                                    else
                                    {
                                        i->data->equation = Utility::convert(i->data->equation, POSTFIX);
                                        i->data->et = POSTFIX;
                                    }
                                    break;
                                }
                            }
                        }
                        
                    }
                }
            }
            // --------------------------------------Remove List Command.
            else if(cmdName == "removelist")
            {
                std::string argument = buffer.substr(pos+1, buffer.find(')')-pos-1);
                if(argument == "postfix")
                {
                    for(auto* i = inputs->getHead(); i != nullptr; i = i->next)
                    {
                        if(i->data->et == POSTFIX)
                        {
                            deleteQueue.push(i);
                        }
                    }
                }
                else if(argument == "prefix")
                {
                    for(auto* i = inputs->getHead(); i != nullptr; i = i->next)
                    {
                        if(i->data->et == PREFIX)
                        {
                            deleteQueue.push(i);
                        }
                    }
                }
                else if(argument == "all")
                {
                    for(auto* i = inputs->getHead(); i != nullptr; i = i->next)
                    {
                        deleteQueue.push(i);
                    }
                }
                else // Should be an integer.
                {
                    int position = std::stoi(argument);
                    if(position < inputs->size())
                    {
                        if(position <= 0)
                        {
                            deleteQueue.push(inputs->getHead());
                        }
                        else
                        {
                            int count = 0;
                            for(auto* i = inputs->getHead(); i != nullptr; i = i->next, count++)
                            {
                                if(count == position)
                                {
                                    deleteQueue.push(i);
                                    break;
                                }
                            }                                
                        }
                    }
                }

            }
            // -------------------------------------- Print List Command
            else if(cmdName == "printlist")
            {
                output << "List:\n";
                if(inputs->size() > 0)
                {
                    for(auto* i = inputs->getHead(); i != nullptr; i = i->next)
                    {
                        output << Utility::typeName(i->data->et) + i->data->equation << "\n";
                    }
                }
                else
                {
                    output << "EMPTY\n";
                }
                
                output << endl;
                
            }
            // -------------------------------------- Print Backwards Command
            else if(cmdName == "printlistbackwards")
            {
                output << "Reversed List:\n";
                if(inputs->size() > 0)
                {
                    for(auto* i = inputs->getTail(); i != nullptr; i = i->prev)
                    {
                        output << Utility::typeName(i->data->et) + i->data->equation << "\n";
                    }
                }
                else
                {
                    output << "EMPTY";
                }
                output << endl;
            }
            /* ---------- ---------------- --------------------- -------------------------------------- ------------------------ ------------ 
            ----------------- ---------------- ------------- RESERVE STACK COMMANDS ------------- ----------------------- ------------------- 
            -------------- -------------------- ---------------- ------------------------------------- -------------------------- ----------- */
            else if(cmdName == "pushreserve")
            {
                std::string argument = buffer.substr(pos+1, buffer.find(')')-pos-1);
                if(argument == "postfix")
                {
                    for(auto* i = inputs->getHead(); i != nullptr; i = i->next)
                    {
                        if(i->data->et == POSTFIX)
                        {
                            reserveStack.push(i->data);
                            deleteQueue.push(i);
                        }
                    }
                }
                else if(argument == "prefix")
                {
                    for(auto* i = inputs->getHead(); i != nullptr; i = i->next)
                    {
                        if(i->data->et == PREFIX)
                        {
                            reserveStack.push(i->data);
                            deleteQueue.push(i);
                        }
                    }
                }
                else if(argument == "all")
                {
                    for(auto* i = inputs->getHead(); i != nullptr; i = i->next)
                    {
                        reserveStack.push(i->data);
                        deleteQueue.push(i);
                    }
                }
                else // Should be an integer.
                {
                    int position = std::stoi(argument);
                    if(position < inputs->size())
                    {
                        if(position <= 0)
                        {
                            reserveStack.push(inputs->getHead()->data);
                            deleteQueue.push(inputs->getHead());
                        }
                        else
                        {
                            int count = 0;
                            for(auto* i = inputs->getHead(); i != nullptr; i = i->next, count++)
                            {
                                if(count == position)
                                {
                                    reserveStack.push(i->data);
                                    deleteQueue.push(i);
                                }
                            }
                        }
                    }
                }

            }
            else if(cmdName == "popreserve")
            {
                int argument = std::stoi(buffer.substr(pos+1, buffer.find(')')-pos-1));
                int position;
                if(argument <= 0)
                {
                    inputs->addFront(reserveStack.top());
                    reserveStack.pop();
                }  
                else if(argument >= inputs->size())
                {
                    inputs->addNode(reserveStack.top());
                    reserveStack.pop();
                }
                else
                {
                    int count = 0;
                    for(auto* i = inputs->getHead(); i != nullptr; i = i->next, count++)
                    {
                        if(count == argument)
                        {
                            auto temp = new Node<InputLine*>(reserveStack.top());
                            reserveStack.pop();

                            temp->prev = i;
                            temp->next = i->next;

                            i->next->prev = temp;
                            i->next = temp;
                            break;
                        }
                    }
                }
            }
            else if(cmdName == "flipreserve")
            {
                LListStack<InputLine*> tempStack;
                while(reserveStack.size() > 0)
                {
                    tempStack.push(reserveStack.top());
                    reserveStack.pop();
                }
                reserveStack.container = tempStack.container;
            }
            else if(cmdName == "printreservesize")
            {
                output << "Reserve size: " << reserveStack.size() << "\n" << endl;
            }
            else if(cmdName == "convertreserve")
            {
                auto temp = reserveStack.top();
                reserveStack.pop();
                temp->et = temp->et == POSTFIX ? PREFIX : POSTFIX;
                temp->equation = Utility::convert(temp->equation, temp->et);
                
                reserveStack.push(temp);
            }
            else if(cmdName == "printreservetop")
            {
                if(reserveStack.size() > 0)
                {
                    output << "Top of Reserve: " <<  Utility::typeName(reserveStack.top()->et) + reserveStack.top()->equation << "\n" << endl;
                }
                else
                {
                    output << "Top of Reserve: EMPTY" << endl;
                }
                
            }
            else if(cmdName == "emptyreserve")
            {
                int argument = std::stoi(buffer.substr(pos+1, buffer.find(')')-pos-1));
                if(argument <= 0)
                {
                    while(reserveStack.size() > 0)
                    {
                        inputs->addFront(reserveStack.top());
                        reserveStack.pop();
                    }
                }  
                else if(argument >= inputs->size())
                {
                    while(reserveStack.size() > 0)
                    {
                        inputs->addNode(reserveStack.top());
                        reserveStack.pop();
                    }
                }
                else
                {
                    while(reserveStack.size() > 0)
                    {
                        int count = 0;
                        for(auto* i = inputs->getHead(); i != nullptr; i = i->next, count++)
                        {
                            if(count == argument)
                            {
                                auto temp = new Node<InputLine*>(reserveStack.top());
                                reserveStack.pop();

                                temp->prev = i;
                                temp->next = i->next;

                                i->next->prev = temp;
                                i->next = temp;
                                break;
                            }
                        }
                    }
                }
            }
            else
            {
                cout << "Invalid command specified [ln #" << line << "]" << endl;
            }
            // Process deleting..
            while(!deleteQueue.empty()) {
                inputs->deletePtr(deleteQueue.front());
                deleteQueue.pop();
            }
        }
        line++;
    }
}

int main(int argc, char* argv[])
{
    if(argc < 2)
    {
        std::cout << "Invalid parameters. Please specifiy I/O files" << std::endl;
        return 0;
    }


    // Parse arguments into a mapable format.
    ArgumentManager am(argc, argv);

    fstream ifs(am.get("input"), fstream::in);
    fstream cmdfs(am.get("command"), fstream::in);
    fstream ofs(am.get("output"), fstream::out);


    std::string buffer;

    LList<InputLine*>* inputs = new LList<InputLine*>();
    /** Input Parsing **/
    while(getline(ifs, buffer))
    {
        if(!buffer.empty())
        {
            InputLine* inp = new InputLine;
            buffer = sanitizedInput(buffer);
            size_t colon = buffer.find(':');
            string eqType = buffer.substr(0, colon);

            if(eqType == "postfix")
                inp->et = POSTFIX;
            else if(eqType == "infix")
                inp->et = INFIX;
            else if(eqType == "prefix")
                inp->et = PREFIX;
            else
                inp->et = NONE;

            inp->equation = buffer.substr(colon+1);
            inputs->addNode(inp);
        }
    }

    ProcessCommands(inputs, cmdfs, ofs);
}


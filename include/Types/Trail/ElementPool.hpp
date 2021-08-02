#include <stack>
#include "Element.hpp"

class ElementPool {

    private: std::stack<Element*> stack = {};
    private: int count = 0;
    
    public:
        int get_count() { return count; }

        ElementPool(int preCount)
        {
            for (int i = 0; i < preCount; i++)
            {
                stack.push(new Element());
                count++;
            }
        }

        ~ElementPool()
        {
            while (stack.size() > 0)
            {
                delete (stack.top());
                stack.pop();
            }
        }

        Element* Get()
        {
            Element* element;
            if (stack.size() == 0)
            {
                element = new Element();
                count ++;
            }
            else 
            {
                element = stack.top();
                stack.pop();
            }
            return element;
        }

        void Release(Element* element)
        {
            stack.push(element);
        }
};
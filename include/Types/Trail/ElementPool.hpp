#include <stack>
#include "Element.hpp"

struct ElementPool {

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
            Element* element;
            while ((element = stack.top()))
            {
                delete (element);
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
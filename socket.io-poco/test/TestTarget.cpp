#include "TestTarget.h"


TestTarget::TestTarget()
{
	
}

void TestTarget::onUpdate(const void* pSender, Array::Ptr & arg)
{
	std::cout << "<--Received message" << std::endl;
	for(int i=0;i<arg->size();++i)
		std::cout << "\t["<< i << "]" << arg->get(i).toString() << std::endl;
}

void TestTarget::ontestevent(const void* pSender, Array::Ptr& arg)
{
	std::cout << "<--Received event" << std::endl;
	for(int i=0;i<arg->size();++i)
			std::cout << "\t["<< i << "]" << arg->get(i).toString() << std::endl;
}

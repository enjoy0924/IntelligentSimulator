#ifndef __OPER_SAMPLE_H__
#define __OPER_SAMPLE_H__
#include "Oper.h"
class OperSample
{
public:
	OperSample();
	~OperSample();
protected:
	
private:
    std::vector<Oper> _operList;
};

#endif
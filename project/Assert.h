#ifndef _ASSERT_H_
#define _ASSERT_H_

void Assert(char*, int);

#ifdef _DEBUG
#define ASSERT()  Assert(__FILE__, __LINE__) 
#else
#define ASSERT()
#endif	// _DEBUG

#endif	//_ASSERT_H_


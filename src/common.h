/*************************************************************************
	> File Name: common.h
	> Author: daniel.wang
	> Mail: wangzhanoop@163.com 
	> Created Time: 2017年11月20日 星期一 10时37分39秒
	> Brief: 
 ************************************************************************/

#ifndef _COMMON_H_
#define _COMMON_H_

#include <string>
#include <vector>
#include <stdint.h>

namespace ftrl_ps {
//typedef uint64_t feaType_t;
//typedef std::vector<feaType_t> feaVec;
typedef int32_t labelType_t;
typedef float valueType_t;
typedef double dvalueType_t;
typedef float paramType_t;
typedef size_t sizeType_t;

//struct Sample {
//	feaVec feat;
//	labelType_t clk;
//	labelType_t noclk;
//};

struct FTRLEntry {
	valueType_t w;
	valueType_t z;
	valueType_t n;
	FTRLEntry(): w(0.0), z(0.0), n(0.0) {}
};

}

#endif //_COMMON_H_


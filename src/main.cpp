/*************************************************************************
	> File Name: main.cpp
	> Author: daniel.wang
	> Mail: wangzhanoop@163.com 
	> Created Time: 2017年11月22日 星期三 14时58分11秒
	> Brief: 
 ************************************************************************/

#include "ps/ps.h"
#include "server.h"
#include "work.h"

using namespace ftrl_ps;
int main(int argc, char* argv[]) {
	std::string data_path = "./data";
	StartServer();
	ps::Start();
	RunWorker(data_path);
	ps::Finalize();
	return 0;
}

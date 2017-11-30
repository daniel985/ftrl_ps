/*************************************************************************
	> File Name: work.h
	> Author: daniel.wang
	> Mail: wangzhanoop@163.com 
	> Created Time: 2017年11月20日 星期一 13时37分47秒
	> Brief: 
 ************************************************************************/

#ifndef _WORK_H_
#define _WORK_H_

#include <iostream>
#include <fstream>
#include <dirent.h>
#include "ps/ps.h"
#include "common.h"
#include "io.h"

namespace ftrl_ps {
const float LOG_MAX = 35.0;

dvalueType_t sigmoid(valueType_t x){
	if (x < - LOG_MAX) return 0.0;
	else if (x > LOG_MAX) return 1.0;
	else {
		return 1/(1+exp(-x));
	}
}

void get_file_path(const std::string path, std::vector<std::string> &pathVec) {
	DIR *p_dir;
	struct dirent *p_dirent;
	if(NULL == (p_dir=opendir(path.c_str()))) {
		LOG(ERROR) << "Open data path error: " << path;
		return;
	}
	while((p_dirent = readdir(p_dir))) {
		std::string sample_path = path + "/" + p_dirent->d_name;
		pathVec.push_back(sample_path);
	}
	closedir(p_dir);
}

void RunWorker(const std::string path) {
	if (!ps::IsWorker()) return;
	ps::KVWorker<valueType_t> w(0);
	std::vector<std::string> pathVec;
	get_file_path(path, pathVec);

	std::cout << "worker id: " << ps::MyRank();
	std::ifstream fin;
	for(sizeType_t i = 0; i < pathVec.size(); i++) {
		fin.open(pathVec[i], std::ios::in);
		if (!fin.is_open()) {
			LOG(ERROR) << "data path is empty" << pathVec[i];
			return;
		}
		std::string line;
		while(getline(fin,line)) {
			std::vector<std::string> SamVec = splitString(line, "\t");
			if (3 > SamVec.size()) {
				LOG(ERROR) << "parse sample line failed: " << line;
				continue;
			}
			std::vector<std::string> feaVec = splitString(SamVec[0], " ");
			labelType_t pv = atoi(SamVec[1].c_str());
			labelType_t clk = atoi(SamVec[2].c_str());
			labelType_t noclk = pv - clk;
			std::vector<ps::Key> feaIdx; 
			for (sizeType_t j = 0; j < feaVec.size(); j++) {
				feaIdx.push_back(atoi(feaVec[j].c_str()));
			}
			std::vector<valueType_t> Wgt;
			w.Wait(w.Pull(feaIdx, &Wgt));
			dvalueType_t Wsum = 0.0;
			for (sizeType_t j = 0; j < feaVec.size(); j++) {
				Wsum += Wgt[j];
			}
			valueType_t p = sigmoid(Wsum);
			valueType_t grad = noclk * p - clk * (1-p);
			std::vector<valueType_t> gradVec;
			for (sizeType_t j = 0; j < feaVec.size(); j++) {
				gradVec[j] = grad;
			}
			w.Wait(w.Push(feaIdx, gradVec));
		}
	}
	fin.close();
}

}
#endif //_WORK_H_

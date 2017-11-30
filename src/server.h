/*************************************************************************
	> File Name: server.h
	> Author: daniel.wang
	> Mail: wangzhanoop@163.com 
	> Created Time: 2017年11月20日 星期一 16时19分40秒
	> Brief: 
 ************************************************************************/

#ifndef _SERVER_H_
#define _SERVER_H_

#include <cmath>
#include "ps/ps.h"
#include "ps/kv_app.h"
#include "common.h"

namespace ftrl_ps {

valueType_t sgn(valueType_t value) {
	if (value < 0) {
		return -1;
	} else if (value > 0) {
		return 1.0;
	} else {
		return 0.0;
	}
}

template <class Val>
class FTRLHandle {
	private:
		paramType_t _alpha = 1.0;
		paramType_t _beta = 1.0;
		paramType_t _l1 = 1.0;
		paramType_t _l2 = 1.0;
		std::unordered_map<ps::Key, FTRLEntry> store;
	
	public:

		void operator() (const ps::KVMeta &req_meta, const ps::KVPairs<Val> &req_data, ps::KVServer<Val> *server) {
			size_t n = req_data.keys.size();
			if (req_meta.push) {
				//ps::CHECK_EQ(n, req_data.vals.size());
				valueType_t grad = req_data.vals[0];
				for (size_t i = 0; i < n; i++) {
					ps::Key key = req_data.keys[i];
					std::unordered_map<ps::Key, FTRLEntry>::const_iterator pw = store.find(key);
					FTRLEntry wgt = pw->second;
					valueType_t sigma = (sqrt(wgt.n + grad*grad) - sqrt(wgt.n)) / _alpha;
					wgt.z += grad - sigma * wgt.w;
					wgt.n += grad * grad;
					if (fabs(wgt.z) < _l1) {
						wgt.w = 0.0;
					} else {
						valueType_t sign = sgn(wgt.z);
						wgt.w = (sign *_l1 - wgt.z) /((_beta + sqrt(wgt.n))/ _alpha + _l2);
					}
					store[key] = wgt;
				}
			} else {
				ps::KVPairs<Val> res;
				res.keys = req_data.keys;
				res.vals.resize(n);
				for (size_t i = 0; i < n; i++) {
					ps::Key key = req_data.keys[i];
					std::unordered_map<ps::Key, FTRLEntry>::const_iterator pw = store.find(key);
					if (store.end() == pw) {
						store[key] = FTRLEntry();
						res.vals[i] = 0.0;
					} else {
						FTRLEntry wgt = pw->second;
						res.vals[i] = wgt.w;
					}
				}
				server->Response(req_meta, res);
			}
		}
};

void StartServer() {
	if (!ps::IsServer()) return;
	auto server = new ps::KVServer<valueType_t>(0);
	server->set_request_handle(FTRLHandle<valueType_t>());
	ps::RegisterExitCallback([server](){ delete server; });
}

}
#endif //_SERVER_H_

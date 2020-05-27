#include<iostream>
#include<stdio.h>
#include<stdlib.h>
#include<vector>
#include<string>
#include<iomanip>
using namespace std;

class Banker {
public:
	vector<string> _pro_name;//j进程名
	vector<vector<size_t>> _max;//最大需求矩阵
	size_t pid = 0;//进程数
	size_t type = 0;//资源种类
	vector<vector<size_t>> _allocation;
	vector<vector<size_t>> _need;
	vector<size_t> _available;
	vector<size_t> _request;
	size_t request_pid = NULL;
	vector<size_t> v1;
	vector<size_t> v2;
	vector<size_t> tmp1;

	bool securitySearch() {//安全性算法
		vector<bool> finish_state;//进程安全状态
		vector<int> security;//安全序列

		for (int i = 0; i < pid; ++i) {//初始化安全状态，全部为false
			bool state = false;
			finish_state.push_back(state);
		}

		vector<size_t> work;//work向量
		work = _available;

		/*从第0个进程开始找，先找第0 个，如果不是就往下找第1个，直到找到符合安全要求的进程，如果找完还没有找到那么就找不到安全序列*/
		vector<int> flag;//进程状态标志
		for (int i = 0; i < pid; ++i) {
			if (flag.size() == type) {
				break;
			}
			else {
				flag.clear();
			}
			int j = 0;
			while (_available[j] >= _need[i][j]) {
				int resouseflag = 0;
				if (_available[j] >= _need[i][j]) {
					resouseflag = 1;
					flag.push_back(resouseflag);
				}
				else if (!(_available[j] >= _need[i][j])) {
					break;
				}

				++j;
				if (j >= type) {
					break;
				}
			}
		}

		//自己写的代码mmp为啥不写注释自己都看不懂。。。。
		if (flag.size() < type) {//进程不全为true 那就是系统状态不安全
			cout << "可用资源不满足任一进程要求,系统陷入不安全状态，不分配资源" << endl;
			for (int i = 0; i < pid; ++i) {//把之前试分配的资源还回去
				for (int j = 0; j < type; ++j) {
					if (i == request_pid) {

						_allocation[request_pid][j] = _allocation[request_pid][j] - _request[j];
						_available[j] = _available[j] + _request[j];
						tmp1[j] = tmp1[j] + _request[j];
						_need[request_pid] = tmp1;
					}
					else {
						break;
					}
				}
			}

			cout << "原矩阵不变:" << endl;
			cout << "need矩阵：" << endl;
			for (auto & a : _need) {
				for (auto & e : a) {
					cout << e << ' ';
				}
				cout << endl;
			}

			cout << "allocation矩阵：" << endl;
			for (auto & a : _allocation) {
				for (auto & e : a) {
					cout << e << ' ';
				}
				cout << endl;
			}

			cout << "available: " << endl;
			for (auto & a : _available) {
				cout << a << ' ';
			}
			cout << endl;

			cout << endl;
			return false;
		}
		else {//这是表示系统安全 可以分配资源
			for (int i = 0; i < pid; i++) {
				while (1) {
					if (finish_state[i] == true) {//进程状态为true就跳出 找下一个进程
						break;
					}
					else {
						vector<size_t> tmp = work;
						int k = 0;
						if (_need[i][k] <= tmp[k]) {//need<=work 分配
							while (_need[i][k] <= tmp[k]) {
								tmp[k] = tmp[k] + _allocation[i][k];//分配资源
								++k;
								if (k == tmp.size()) {
									break;
								}
							}
							work = tmp;
							finish_state[i] = true;
							security.push_back(i);
							i = -1;
							break;
						}
						else {//need>work 继续找
							work = tmp;
							finish_state[i] = false;
							break;
						}
					}
					break;
				}
			}

			if (security.size() == pid) {
				cout << "安全" << endl;
				printf("安全序列为：\n");
				for (auto & i : security) {
					cout << i << ' ';
				}
				cout << endl;
				return true;
			}
			else {
				cout << "系统不安全" << endl;
				cout << endl;
				return false;
			}
		}
	}

	void requestSourse() {
		vector<vector<size_t>> allocation = _allocation;
		vector<vector<size_t>> need = _need;
		vector<size_t> available = _available;
		vector<size_t> zflag;//零标志 我当时用它干啥来着？？？

		//进程提出请求
		for (int i = 0; i < pid; i++) {//选择要请求资源的进程
			if (i == request_pid) {
				tmp1 = _need[i];
				break;
			}
			else {
				continue;
			}
		}

		for (int i = 0; i < type; i++) {//试分配资源
			if (_request[i] <= tmp1[i]) {//request<=need
				if (_request[i] <= available[i]) {//requst <= available
					while (1) {
						int j = 0;
						while (_request[j] <= _available[j] && _request[j] <= _need[request_pid][j]) {
							if (_request[j] <= _available[j] && _request[j] <= need[request_pid][j]) {
								allocation[request_pid][j] = allocation[request_pid][j] + _request[j];
								available[j] = available[j] - _request[j];
								tmp1[j] = tmp1[j] - _request[j];
								need[request_pid] = tmp1;
							}
							else {
								goto AOF;
								break;
							}

							++j;
							if (j >= type) {
								break;
							}
						}	
						if (_request> _available) goto AOF;
						else if (_request > _need[request_pid]) goto NOF;
						break;
					}

					while (1) {//这个循环是用来判断need=0的情况
						int j = 0;
						int resouse_zf = 0;
						while (need[request_pid][j] == 0) {
							if (need[request_pid][j] == 0) {
								resouse_zf = 1;
								zflag.push_back(resouse_zf);
							}
							++j;
							if (j >= type) {
								break;
							}
						}

						if (zflag.size() == type) {
							for (int k = 0; k < type; ++k) {
								available[k] = available[k] + allocation[request_pid][k];
							}
						}
						break;
					}
					_need = need;
					_available = available;
					_allocation = allocation;

					cout << "success，更新矩阵数据：" << endl;//试分配成功
					cout << "need矩阵：" << endl;
					for (auto & a : _need) {
						for (auto & e : a) {
							cout << e << ' ';
						}
						cout << endl;
					}

					cout << "allocation矩阵：" << endl;
					for (auto & a : _allocation) {
						for (auto & e : a) {
							cout << e << ' ';
						}
						cout << endl;
					}

					cout << "available: " << endl;
					for (auto & a : _available) {
						cout << a << ' ';
					}
					cout << endl;
					securitySearch();
					break;
				}
				else {
				AOF:	cout << "request failed!请求资源>available,原矩阵不变" << endl;
					cout << "need矩阵：" << endl;
					for (auto & a : _need) {
						for (auto & e : a) {
							cout << e << ' ';
						}
						cout << endl;
					}

					cout << "allocation矩阵：" << endl;
					for (auto & a : _allocation) {
						for (auto & e : a) {
							cout << e << ' ';
						}
						cout << endl;
					}

					cout << "available: " << endl;
					for (auto & a : _available) {
						cout << a << ' ';
					}
					cout << endl;
					break;
				}
			}
			else {
				NOF: cout << "request failed！请求资源>need，原矩阵不变" << endl;
				cout << "need矩阵：" << endl;
				for (auto & a : _need) {
					for (auto & e : a) {
						cout << e << ' ';
					}
					cout << endl;
				}

				cout << "allocation矩阵：" << endl;
				for (auto & a : _allocation) {
					for (auto & e : a) {
						cout << e << ' ';
					}
					cout << endl;
				}

				cout << "available: " << endl;
				for (auto & a : _available) {
					cout << a << ' ';
				}
				cout << endl;
				break;
			}
		}
	}

	void print() {
		vector<size_t> v1;
		vector<size_t> v2;
	
		printf("\n=========================银行家算法==========================\n");
		printf("输入进程数： \n");
		cin >> pid;
		printf("输入资源种类数： \n");
		cin >> type;

		printf("输入need矩阵：\n");
		for (int i = 0; i < pid; ++i) {
			for (int j = 0; j < type; ++j) {
				int num;
				cin >> num;
				v1.push_back(num);
			}
			_need.push_back(v1);
			v1.clear();
		}

		printf("输入allocation矩阵：\n");
		for (int i = 0; i < pid; ++i) {
			for (int j = 0; j < type; ++j) {
				int num;
				cin >> num;
				v2.push_back(num);
			}
			_allocation.push_back(v2);
			v2.clear();
		}

		printf("输入空闲资源available：");
		for (int i = 0; i < type; ++i) {
			int num;
			cin >> num;
			_available.push_back(num);
		}

		securitySearch();
		while (1) {
			int select;
			printf("输入选项 （1继续、0退出）：\n");
			cin >> select;
			if (select == 1) {
				printf("输入要请求资源的进程：");
				cin >> request_pid;

				printf("输入请求资源request：\n");
				_request.clear();
				for (int i = 0; i < type; ++i) {
					int num;
					cin >> num;
					_request.push_back(num);
				}
				requestSourse();
				continue;
			}
			else if (select == 0) {
				break;
			}
			else {
				cout << "输入错误重新输:" << endl;
				continue;
			}
			break;
		}
	}
};

int main() {
	Banker b;
	b.print();
	return 0;
}
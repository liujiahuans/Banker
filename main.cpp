#include<iostream>
#include<stdio.h>
#include<stdlib.h>
#include<vector>
#include<string>
#include<iomanip>
using namespace std;

class Banker {
public:
	vector<string> _pro_name;//j������
	vector<vector<size_t>> _max;//����������
	size_t pid = 0;//������
	size_t type = 0;//��Դ����
	vector<vector<size_t>> _allocation;
	vector<vector<size_t>> _need;
	vector<size_t> _available;
	vector<size_t> _request;
	size_t request_pid = NULL;
	vector<size_t> v1;
	vector<size_t> v2;
	vector<size_t> tmp1;

	bool securitySearch() {//��ȫ���㷨
		vector<bool> finish_state;//���̰�ȫ״̬
		vector<int> security;//��ȫ����

		for (int i = 0; i < pid; ++i) {//��ʼ����ȫ״̬��ȫ��Ϊfalse
			bool state = false;
			finish_state.push_back(state);
		}

		vector<size_t> work;//work����
		work = _available;

		/*�ӵ�0�����̿�ʼ�ң����ҵ�0 ����������Ǿ������ҵ�1����ֱ���ҵ����ϰ�ȫҪ��Ľ��̣�������껹û���ҵ���ô���Ҳ�����ȫ����*/
		vector<int> flag;//����״̬��־
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

		//�Լ�д�Ĵ���mmpΪɶ��дע���Լ�����������������
		if (flag.size() < type) {//���̲�ȫΪtrue �Ǿ���ϵͳ״̬����ȫ
			cout << "������Դ��������һ����Ҫ��,ϵͳ���벻��ȫ״̬����������Դ" << endl;
			for (int i = 0; i < pid; ++i) {//��֮ǰ�Է������Դ����ȥ
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

			cout << "ԭ���󲻱�:" << endl;
			cout << "need����" << endl;
			for (auto & a : _need) {
				for (auto & e : a) {
					cout << e << ' ';
				}
				cout << endl;
			}

			cout << "allocation����" << endl;
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
		else {//���Ǳ�ʾϵͳ��ȫ ���Է�����Դ
			for (int i = 0; i < pid; i++) {
				while (1) {
					if (finish_state[i] == true) {//����״̬Ϊtrue������ ����һ������
						break;
					}
					else {
						vector<size_t> tmp = work;
						int k = 0;
						if (_need[i][k] <= tmp[k]) {//need<=work ����
							while (_need[i][k] <= tmp[k]) {
								tmp[k] = tmp[k] + _allocation[i][k];//������Դ
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
						else {//need>work ������
							work = tmp;
							finish_state[i] = false;
							break;
						}
					}
					break;
				}
			}

			if (security.size() == pid) {
				cout << "��ȫ" << endl;
				printf("��ȫ����Ϊ��\n");
				for (auto & i : security) {
					cout << i << ' ';
				}
				cout << endl;
				return true;
			}
			else {
				cout << "ϵͳ����ȫ" << endl;
				cout << endl;
				return false;
			}
		}
	}

	void requestSourse() {
		vector<vector<size_t>> allocation = _allocation;
		vector<vector<size_t>> need = _need;
		vector<size_t> available = _available;
		vector<size_t> zflag;//���־ �ҵ�ʱ������ɶ���ţ�����

		//�����������
		for (int i = 0; i < pid; i++) {//ѡ��Ҫ������Դ�Ľ���
			if (i == request_pid) {
				tmp1 = _need[i];
				break;
			}
			else {
				continue;
			}
		}

		for (int i = 0; i < type; i++) {//�Է�����Դ
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

					while (1) {//���ѭ���������ж�need=0�����
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

					cout << "success�����¾������ݣ�" << endl;//�Է���ɹ�
					cout << "need����" << endl;
					for (auto & a : _need) {
						for (auto & e : a) {
							cout << e << ' ';
						}
						cout << endl;
					}

					cout << "allocation����" << endl;
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
				AOF:	cout << "request failed!������Դ>available,ԭ���󲻱�" << endl;
					cout << "need����" << endl;
					for (auto & a : _need) {
						for (auto & e : a) {
							cout << e << ' ';
						}
						cout << endl;
					}

					cout << "allocation����" << endl;
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
				NOF: cout << "request failed��������Դ>need��ԭ���󲻱�" << endl;
				cout << "need����" << endl;
				for (auto & a : _need) {
					for (auto & e : a) {
						cout << e << ' ';
					}
					cout << endl;
				}

				cout << "allocation����" << endl;
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
	
		printf("\n=========================���м��㷨==========================\n");
		printf("����������� \n");
		cin >> pid;
		printf("������Դ�������� \n");
		cin >> type;

		printf("����need����\n");
		for (int i = 0; i < pid; ++i) {
			for (int j = 0; j < type; ++j) {
				int num;
				cin >> num;
				v1.push_back(num);
			}
			_need.push_back(v1);
			v1.clear();
		}

		printf("����allocation����\n");
		for (int i = 0; i < pid; ++i) {
			for (int j = 0; j < type; ++j) {
				int num;
				cin >> num;
				v2.push_back(num);
			}
			_allocation.push_back(v2);
			v2.clear();
		}

		printf("���������Դavailable��");
		for (int i = 0; i < type; ++i) {
			int num;
			cin >> num;
			_available.push_back(num);
		}

		securitySearch();
		while (1) {
			int select;
			printf("����ѡ�� ��1������0�˳�����\n");
			cin >> select;
			if (select == 1) {
				printf("����Ҫ������Դ�Ľ��̣�");
				cin >> request_pid;

				printf("����������Դrequest��\n");
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
				cout << "�������������:" << endl;
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
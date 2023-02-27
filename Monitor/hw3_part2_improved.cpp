#include <iostream>
#include <string>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <vector>


#define N 5
#define M 1000000
#define LEFT (p_i + N - 1)%N
#define RIGHT (p_i + 1)%N
#define THINKING 0
#define HUNGRY 1
#define EATING 2

using namespace std;

mutex m;
condition_variable cond;
char state[N] = {0};
vector<int> queue;

void philosopher(int);
void print_state();
bool fair_check(int);

class Monitor{
public:
	void think(void);
	void take_fork(int p_i);
	void eat(void);
	void put_fork(int p_i);
	bool see(int p_i);
};


Monitor monitor;

int main(void)
{
	int i;
	for(i = 0; i < N; i++) printf("  (%d)     ", i);
	printf("\n");
	print_state();
	int p[N] {0, 1, 2, 3, 4};
	thread P0(philosopher, p[0]);
	i++;
	thread P1(philosopher, p[1]);
	i++;
	thread P2(philosopher, p[2]);
	i++;
	thread P3(philosopher, p[3]);
	i++;
	thread P4(philosopher, p[4]);
	
	P0.join();
	P1.join();
	P2.join();
	P3.join();
	P4.join();
	
	return 0;
}


void Monitor::think(void)
{
	this_thread::sleep_for(chrono::seconds(3));
}

void Monitor::take_fork(int p_i)
{
	unique_lock<mutex> lock(m);
	
	state[p_i] = HUNGRY;
	queue.push_back(p_i);
	print_state();
	
	while(state[LEFT] == EATING || state[RIGHT] == EATING || fair_check(p_i)){
		
		cond.wait(lock);
	}
	
	state[p_i] = EATING;
	int t = 0;
	while(queue[t] != p_i) t++;
	queue.erase(queue.begin() + t);
	print_state();
	
	lock.unlock();
}

void Monitor::eat(void)
{
	this_thread::sleep_for(chrono::seconds(3));
}

void Monitor::put_fork(int p_i)
{
	unique_lock<mutex> lock(m);
	
	state[p_i] = THINKING;
	print_state();
	
	lock.unlock();
	cond.notify_all();
}

bool Monitor::see(int p_i)
{
	return state[LEFT] != EATING && state[RIGHT] != EATING;
}

void philosopher(int p_i)
{
	//int p_i = id;
	while(1){
		
		monitor.think();
		monitor.take_fork(p_i);
		monitor.eat();
		monitor.put_fork(p_i);
	}
}

void print_state()
{	
	for(int i = 0; i < N; i++){
		
		if(state[i] == THINKING) cout << "THINKING  ";
		else if(state[i] == HUNGRY) cout << "\033[33m" << " HUNGRY   " << "\033[0m";
		else if(state[i] == EATING) cout << "\033[34m" << " EATING   " << "\033[0m";
	}
	
	cout << "\twaiting list: ";
	
	for(int i = 0; i < queue.size(); i++) cout << queue[i] << " ";
	cout << endl << endl << flush;
}

bool fair_check(int id)
{
	int t = 0;
	
	while(queue[t] != id) t++;
	
	for(int i = 0; i < t; i++){
		if((queue[i] - queue[t] == 1 && state[ (queue[i]+1)%N ] != EATING) || (queue[i] - queue[t] == -1 && state[ (queue[i]+4)%N ] != EATING)){
			
			return true;
		}
	}
	return false;
}














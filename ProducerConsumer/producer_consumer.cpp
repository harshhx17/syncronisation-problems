#include <iostream>
#include <thread>
#include <queue>
#include <atomic>
#include <mutex>
#include <unistd.h>

using namespace std;
typedef long long ll;

class sem 
{
    private:
		mutex m;
		atomic<ll> val;

	public:
		sem(ll init)
		{
            val = init;
        }

		void wait() 
		{
			while (true) 
			{
				while (val <= 0);
				m.lock();
				if (val <= 0) 
				{ 
					m.unlock();
					continue; 
				}
				val--;
				m.unlock();
				break;
			}
		}

		void signal() 
		{
			val++; 
		}
};

sem contain(0), empt(10);
mutex buf_m;
queue<ll> buf;

void produce()
{
	while(true)
	{
		ll x = rand() % 100;
		empt.wait();
		buf_m.lock();
		buf.push(x);
		cout<<"Writing "<<x<<endl;
		buf_m.unlock();
		contain.signal();
		usleep(100000);
	}
}

void consume(){
	while(true)
	{
		contain.wait();
		buf_m.lock();
		cout<<"reading "<<buf.front()<<endl;
		buf.pop();
		buf_m.unlock();
		empt.signal();
		usleep(200000);
	}
}

signed main()
{
	thread pro(produce), cons(consume);
	pro.join();
	cons.join();
}
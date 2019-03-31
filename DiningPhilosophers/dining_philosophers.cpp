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
		sem(ll init = 1)
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

mutex chop_m;
sem chop[5];
// sem chop[5] = {sem(0),sem(0),sem(0),sem(0),sem(0)};

void phil(ll philNumber){
	ll hunger = rand()%10;
	while(hunger--){
		chop_m.lock();
		chop[philNumber].wait();
		chop[(philNumber+5)%5].wait();
		chop_m.unlock();
		cout<<"philosopher "<<philNumber<<" is dining"<<endl;
		chop[philNumber].signal();
		chop[(philNumber+5)%5].signal();
		usleep(1000);
	}
}

signed main()
{
	// for(ll i=0;i<5;i++)
	// 	chop[i] = sem(0);
	thread p1(phil,0);
	thread p2(phil,1);
	thread p3(phil,2);
	thread p4(phil,3);
	thread p5(phil,4);
	p1.join();
	p2.join();
	p3.join();
	p4.join();
	p5.join();
}
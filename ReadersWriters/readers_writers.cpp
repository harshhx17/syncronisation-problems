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

mutex io_m, print;
sem wri(1);
vector<ll> a;
atomic<ll> readCont{0};

void writer(){
	ll index = 0;
	while(index < 10){
		wri.wait();
		ll item = rand()%100;
		a.push_back(item);
		cout<<"writing "<<item<<endl;
		wri.signal();
		index++;
		usleep(rand()%1000);
	}
}

void reader(ll readerNumber){
	ll index = 0;
	while(index < 10){
		io_m.lock();
		readCont++;
		if(readCont == 1) wri.wait();
		io_m.unlock();
		// Every reader performs his own reading if something is available
		if(a.size() > index)
		{
			print.lock();
			cout<<readerNumber<<" numbered reader is reading "<<a[index]<<endl;
			print.unlock();
			index++;
		}
		io_m.lock();
		readCont--;
		if (readCont == 0) wri.signal();
		io_m.unlock();
		usleep(rand()%1000);
	}
}

signed main()
{
	thread t1(writer);
	thread t2(reader, 0), t3(reader, 1), t4(reader, 2);
	t1.join(); t2.join(); t3.join(); t4.join();
}
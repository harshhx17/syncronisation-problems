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

sem tobaccoMatch(0), matchPaper(0), paperTobacco(0);
mutex smoking;

void agent() 
{
	while(true)
	{
		smoking.lock();
		ll x = rand()%3;
		switch (x)
		{
			case 0:
				tobaccoMatch.signal();
				break;
			case 1:
				matchPaper.signal();
				break;
			case 2:
				paperTobacco.signal();
		}
		smoking.unlock();
		sleep(1);
	}
}

void tobacco()
{
	while(true)
	{
		matchPaper.wait();
		smoking.lock();
		cout<<"The tobacco smoker has made the cigarette"<<endl;
		smoking.unlock();
		sleep(0.1);
	}
}
void paper()
{
	while(true)
	{
		tobaccoMatch.wait();
		smoking.lock();
		cout<<"The paper smoker has made the cigarette"<<endl;
		smoking.unlock();
		sleep(0.1);
	}

}
void match()
{
	while(true)
	{
		paperTobacco.wait();
		smoking.lock();
		cout<<"The match smoker has made the cigarette"<<endl;
		smoking.unlock();
		sleep(0.1);
	}
}
signed main()
{
	thread ag(agent);
	thread smoker1(tobacco);
	thread smoker2(paper);
	thread smoker3(match);
	ag.join();
	smoker1.join();
	smoker2.join();
	smoker3.join();
}
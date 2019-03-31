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

ll freeWaitingRoomSeats = 3;
sem barberReady(1), customerReady(0), waitingRoom_m(1);

void barber()
{
	while(true)
	{	
		if(freeWaitingRoomSeats == 3)
		{
			cout<<"Barber is sleeping"<<endl;
		}
		customerReady.wait();
		waitingRoom_m.wait();
		freeWaitingRoomSeats++;
		waitingRoom_m.signal();
		cout<<"Cutting hair of customer"<<endl;
		barberReady.signal();
	}
}

void customer()
{
	while(true)
	{
		ll x = rand()%10;
		ll y = rand()%4;
		sleep(y + x/10.0);
		waitingRoom_m.wait();
		cout<<"New customer arrived"<<endl;
		if(freeWaitingRoomSeats > 0)
		{
			freeWaitingRoomSeats--;
			cout<<"Vacant seat available"<<endl;
			customerReady.signal();
			waitingRoom_m.signal();
			barberReady.wait();
		}
		else
		{
			cout<<"No available seat for the customer"<<endl;
			waitingRoom_m.signal();
		}
	}
}

signed main()
{
	thread barb(barber);
	thread customer1(customer);
	thread customer2(customer);
	thread customer3(customer);
	thread customer4(customer);
	barb.join();
	customer1.join();
	customer2.join();
	customer3.join();
	customer4.join();
}
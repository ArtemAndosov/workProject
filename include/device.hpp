#include <HardCommand.hpp>
#include <queue>
#include <chrono>
#include <thread>
#include <mutex>
class device{
    public:
    std::queue<HardCommand>* m_pQueue;
    std::mutex* mute;
  int m_ID;   
  int m_array[10];
 
 void m_generate_massive(){
srand(time(0));
for(int i=0; i<10; i++)
m_array[i] = 1 + rand() % 5;
};

 void m_create_Hc(){
       while (true){
    m_generate_massive();
    mute->lock();
    HardCommand Hc1(m_ID,m_array);
    m_pQueue->push(Hc1);
    mute->unlock();
    std::this_thread::sleep_for(std::chrono::seconds(1));
   };};


void m_start() {
    std::thread create_Hc(&device::m_create_Hc, this);
    create_Hc.detach();
    };

device(int a){
    this->m_ID = a;
    }

 device () = default;
 ~device () = default;   
};
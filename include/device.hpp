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
 
 void generate_massive(){
srand(time(0));
for(int i=0; i<10; i++)
m_array[i] = 1 + rand() % 5;
};

 void create_Hc(){
       while (true){
    generate_massive();
    HardCommand Hc1(m_ID,m_array);
    //mute->lock();
    m_pQueue->push(Hc1);
    //mute->unlock();
    std::this_thread::sleep_for(std::chrono::seconds(1));
   };};


void start() {
    std::thread create_Hc(&device::create_Hc, this);
    create_Hc.join();
    };

device(int a){
    this->m_ID = a;
    }
 device () = default;
 ~device () = default;   
};
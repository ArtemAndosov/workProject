#include <HardCommand.hpp>
#include <queue>

class device{
    public:
    std::queue<HardCommand>* m_pQueue;
 int m_ID;   
 int m_array[10];
void generate_massive(){
srand(time(0));
for(int i=0; i<10; i++)
m_array[i] = 1 + rand() % 5;
};

void create_Hc(){
    generate_massive();
    HardCommand Hc1(m_ID,m_array);
    m_pQueue->push(Hc1);
   // Hc1.print();


};
device(int a){
    this->m_ID = a;
    }
 device () = default;
 ~device () = default;   
};
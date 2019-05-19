#include <vector>

class Queue
{
private:
    // typedef void(*ptr)(int);
    // typedef struct s_queueItem {
    //     ptr queueFunc;
    //     int argument;
    // } queueItem_t;
    std::vector<void(*)(int)> vqueue;
    std::vector<int> arguments;
    //std::vector<queueItem_t> vqueue;
    bool* monitoringValue;

public:
    Queue();
    void update();
    void addToQueue(void (*ptr)(int), int argument);
    void addMeasuingValue(bool* _monitoringValue);
};

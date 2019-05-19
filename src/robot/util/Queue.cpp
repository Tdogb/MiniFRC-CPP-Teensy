#include "Queue.h"
#include <stddef.h>
#include <Arduino.h>

Queue::Queue() {}

void Queue::update() {
    if((!vqueue.empty()) && !(*monitoringValue)) {
        (*vqueue.front())(arguments.front());
        vqueue.erase(vqueue.begin());
        arguments.erase(arguments.begin());
    }
}

void Queue::addToQueue(void (*ptr)(int), int argument) {
    vqueue.push_back(ptr);
    arguments.push_back(argument);
}

void Queue::addMeasuingValue(bool* _monitoringValue) {
    monitoringValue = _monitoringValue;
}
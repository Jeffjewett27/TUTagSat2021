void minuteLoop();
void subscribeInitialize(void(*func)());
void subscribeEveryTwoSec(void(*func)(int i));
void subscribeEveryFourSec(void(*func)(int i));
void subscribeEveryEightSec(void(*func)(int i));
void subscribeCleanup(void(*func)());

void subscribeEveryTwoSecAfter(void(*func)(int i));
void subscribeEveryFourSecAfter(void(*func)(int i));

void callEachAction(void(**actions)(), int num);
void callEachSubscriber(void(**subscribers)(int iter), int num, int val, uint16_t mask);
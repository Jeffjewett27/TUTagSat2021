void minuteLoop();
void subscribeInitialize(void(*func)()); //initializes each minute
void subscribeEveryTwoSec(void(*func)(int i)); //use for 1 byte values
void subscribeEveryFourSec(void(*func)(int i)); //use for 2 byte values
void subscribeEveryEightSec(void(*func)(int i)); //use for 4 byte values
void subscribeCleanup(void(*func)()); //called after minute is up

void subscribeEveryTwoSecAfter(void(*func)(int i)); //for pulse counts
void subscribeEveryFourSecAfter(void(*func)(int i)); //for pulse counts

void callEachAction(void(**actions)(), int num);
void callEachSubscriber(void(**subscribers)(int iter), int num, int val, uint16_t mask);
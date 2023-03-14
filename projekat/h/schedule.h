class PCB;
class Scheduler {
public:
	static void put(PCB*);
	static PCB* get();
};


#include <string>
using namespace std;

class LIST;
class EVENT;

class LLNODE {
	friend class LIST; 
	
	private:
		LLNODE *next;
		EVENT  *ev;
		
	public:
		LLNODE(EVENT *e);
        ~LLNODE();
};

class LIST {
	private:
		LLNODE *front;
		
	public:
		LIST();
		~LIST();
		void display();
		EVENT* getFirstEvent();
		void removeFirstEvent();
		void insertEvent(EVENT *e);
        void removeEventsNamed(string n); 
};

class EVENT {
	private:
		string device;
		int  value;
		int  processTime;
		
	public:
		EVENT(string n, int v, int pt);
		void display();
		int  getProcessTime();
        string getDeviceName();
        int getValue();         
};


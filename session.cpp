#include <iostream>
#include <fstream>
#include <vector>
#include <string>
using namespace std;

/* Please provide the name of the text file as a command line arguement */

class Schedule {
    vector<string> name;            // store name of session
    vector<pair<int,int>> session;  // pair storing start & end of session
    vector<bool> slotBooked;        // bool to store booking of a slot
    int maxsession;                 // max number of sessions possible
public:
    Schedule(string str) {     // Constructor
        maxsession = 0;

        vector<int> start;    
        vector<int> duration;
        ifstream read;
        read.open(str);
        int index = 0;
        while(!read.eof()){
            if(!(index%3)){
                string dummy;
                read >> dummy;
                name.push_back(dummy);
            }
            else{
                int dummy;
                read >> dummy;
                if((index%3)==1) start.push_back(dummy);
                else duration.push_back(dummy);
            }
            index++;
        }
        read.close();

        for(int i = 0; i < name.size(); i++){
            session.push_back(make_pair(start[i],start[i]+duration[i]-1));
        }

        for(int i = 0; i < 5000; i++){   // set all slots to be free initially
            slotBooked.push_back(false);
        }
    }

    void sort(){    // sort in ascending order of starting time
        for(int i = 0; i < session.size()-1; i++){
            int minIndex = i;
            for(int j = i+1; j < session.size(); j++){
                if(session[j].first <= session[minIndex].first) minIndex = j;
                swap(session[i],session[minIndex]);
            }
        } 
    }

    bool Booked(int start, int end){   //check if all slots are booked of a particular timing
        for(int i = start; i <= end; i++){
            if(slotBooked[i] == true) return true;
        }
        return false;
    }

    void doBooking(int start, int end){  // book some particular slots
        for(int i = start; i <= end; i++){
            slotBooked[i] = true;
        }
    }

    int max_dur(){   // function to obtain session with max duration 
        int max = 0;
        for(int i = 0; i < name.size(); i++){
            if(session[i].second-session[i].first > max) {
                max = session[i].second-session[i].first;
            }
        }
        return max+1;
    }

    int check(int n){   // check all sessions for attending them
        if(n == max_dur()) return maxsession;

        for(int i = 0; i < name.size(); i++){
            if((session[i].second-session[i].first == n) && !(Booked(session[i].first,session[i].second))){ 
                maxsession++;
                doBooking(session[i].first,session[i].second);
            }
        }

        n++;
        return check(n);
    }
};

int main(int argc, char *argv[]) {

    Schedule schedule(argv[1]);
    schedule.sort();
    cout << "Maximum Number of sessions that can be attended : " << schedule.check(0) << endl;

}
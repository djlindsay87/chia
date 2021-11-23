#include <chrono>
#include <iostream>
#include <thread>
#include <fstream>
#include <string>
#include <ctime>
#include <io.h>
#include <math.h>
#include <vector>

using namespace std;
using namespace chrono_literals;

void datifier(long int s);
void chiaLogger();
int chiaReader();
int growthLogger();
bool options();

int main(int argc, char *argv[])
{	
	
	cout<<"You have a chia pet.\n";
	do{	
		if (growthLogger()==-2){
			cout<<"It's dead. Clean and start over :(\n";
		}
		else if (growthLogger()==0){
			cout<<"You haven't seeded it yet.\n";
		}
		else if (growthLogger()==1){
			cout<<"You should water it...\n";
		}
		else if (growthLogger()==2){
			cout<<"Nothing's happened yet.\n";
		}
		else if (growthLogger()==3){
			cout<<"It's sprouted!\n";
		}
		else if (growthLogger()==4){
			cout<<"It's got short growth on it!\n";
		}
		else if (growthLogger()==5){
			cout<<"It's a nice length!\n";
		}
		else if (growthLogger()==6){
			cout<<"It's getting really long!\n";
		}
		else if (growthLogger()==7){
			cout<<"It's lasted its life and is leggy and dead. Good job!\n";
		}
		options();
	}while (options()!=true);
		
   return 0;
}

void datifier(long int s){
	if (s>=60*60*24*7*52){//years
		cout<<floor(s/(60*60*24*7*52))<<" years, "<<int(floor(s/(60*60*24*7)%52))<<" weeks, "<<int(floor(s/(60*60*24)))%7<<" days, "<<int(floor(s/(60*60)))%24<<" hours, "<<int(floor(s/60))%60<<" minutes, "<< s%60<<" seconds";
	}else if (s>=60*60*24*7){//weeks
		cout<<floor(s/(60*60*24*7))<<" weeks, "<<int(floor(s/(60*60*24)))%7<<" days, "<<int(floor(s/(60*60)))%24<<" hours, "<<int(floor(s/60))%60<<" minutes, "<< s%60<<" seconds";
	}else if(s>=60*60*24){//days
		cout<<floor(s/(60*60*24))<<" days, "<<int(floor(s/(60*60)))%24<<" hours, "<<int(floor(s/60))%60<<" minutes, "<< s%60<<" seconds";
	}else if(s>=60*60){//hours
		cout<<floor(s/(60*60))<<" hours, "<<int(floor(s/60))%60<<" minutes, "<< s%60<<" seconds";
	}else if(s>=60){//minutes
		cout<<floor(s/60)<<" minutes, "<< s%60<<" seconds";
	}else{cout<<s<<" seconds";}
	return;
}

void chiaLogger(){ //writes to chialog.txt //NECESSARY
	chrono::system_clock::time_point append_point = chrono::system_clock::now();
	time_t at = chrono::system_clock::to_time_t(append_point);
		
	fstream appendFile;
	appendFile.open("chialog.txt", fstream::app|fstream::out);
	
	if (!appendFile.is_open()){cout<<"Unable to write file!\n";} 
	else
	{
		appendFile<<at<<endl;
		appendFile.close();
	}
	return;
}

int chiaReader(){ //returns duration from now and last action
	vector<int> logData;
	
	ifstream readFile("chialog.txt");
	int temp;
	
	if (!readFile.is_open()){cout<<"Unable to read file!\n";}
	else 
	{	//adds all logs to vector
		while (readFile>>temp){logData.push_back(temp);}
	}
	
	if (logData.size()>=1)
	{
		//gets first soak or seed or last water
		chrono::system_clock::time_point soak_point = chrono::system_clock::from_time_t(logData.back());
		//gets now
		chrono::system_clock::time_point now_point = chrono::system_clock::now();
		long int s = chrono::duration_cast<chrono::seconds>(now_point-soak_point).count();
		return s;
	}
	
	else return -1;
}

int growthLogger(){ //provides the state of the chia pet (seeded, soaked, watered, etc.)
	vector<int> logData;
	chrono::system_clock::time_point now_point = chrono::system_clock::now();
	
	if( _access( "chialog.txt", 00 ) != -1){
		ifstream readFile("chialog.txt");
		int temp;
		
		if (!readFile.is_open()){cout<<"Unable to read file(g)!\n";}
		else 
		{	//adds all logs to vector
			while (readFile>>temp){logData.push_back(temp);}
		}
		
		if (logData.size()==1){return 0;} //soaked
		
		if (logData.size()==2){return 1;} //seeded
		
		else if (chiaReader()>(60*60*48)&&logData.size()>1){return -2;}
		
		if (logData.size()>=3){
			chrono::system_clock::time_point first_water = chrono::system_clock::from_time_t(logData[2]);
			long int s= chrono::duration_cast<chrono::seconds>(now_point-first_water).count();
		
			if (s<(60*60*48)) //watered 1st fill
				{return 2;}
			else if (s>=(60*60*48)&&s<(60*60*84)) //sprout phase 1.5 days
				{return 3;}
			else if (s>=(60*60*84)&&s<(60*60*132)) //short phase 2 days
				{return 4;}
			else if (s>=(60*60*132)&&s<(60*60*300)) //med phase 7 days
				{return 5;}
			else if (s>=(60*60*300)&&s<(60*60*444)) //long phase 6 days
				{return 6;}
			else if (s>=(60*60*444)) //dead
				{return 7;}
		}
	}
	return -1;
}

bool options(){	
	bool seeded;
	bool soaked;
	if( _access( "chialog.txt", 00 ) != -1 && growthLogger()>=0){
		soaked=true;
	}else soaked=false;

	if( _access( "chialog.txt", 00 ) != -1 && growthLogger()>=1){
		seeded=true;
	}else seeded=false;

	bool watered=false;
	bool sprouted = false;
	bool grown=false;
	
	string action;
	getline(cin, action);
	cin.clear();
	
	if (action == "soak"&&growthLogger()!=-2&&growthLogger()!=7){
		if (soaked==false){
			chiaLogger();
			cout<<"You soaked your chia pet! Soon you will be able to seed it!\n";
		} else {
			cout<<"You already soaked your chia pet!\n";
			datifier(chiaReader());
			cout<<" since soaked!\n";
		}
		return false;
	}
	
	else if (action == "seed"&&growthLogger()!=-2&&growthLogger()!=7){
		if (_access( "chialog.txt", 00 ) != -1 && seeded==false && chiaReader()>=(30*60)){
			chiaLogger();
			cout<<"You seeded your chia pet!\n";
		} 
		else if(seeded==true){
			cout<<"You already seeded your chia pet!\n";
			datifier(chiaReader());
			cout<<" since seeded!\n";
		}
		else if(_access( "chialog.txt", 00 ) == -1){
			cout<<"You must soak your chia pet first!\n";
		}
		else{
			cout<<"You must soak your chia pet for 30 minutes!\n";
			datifier(chiaReader());
			cout<<" since soaked!\n";
		}
		return false;
	}
	
	else if (action == "water"&&growthLogger()!=-2&&growthLogger()!=7){//||action=="Water"||action=="WATER"){
		if (seeded==true){
			if (growthLogger()==1){
				cout<<"You watered your chia pet!\n";
				datifier(chiaReader());
				cout<<" since seeded!\n";
				chiaLogger();
			}
			
			else if (growthLogger()>=2&&chiaReader()>(60*60*24)){
				cout<<"You watered your chia pet!\n";
				//reads out last watering
				datifier(chiaReader());
				cout<<" since last watered!\n";
				chiaLogger();
			} 
			else {
				cout<<"You already watered today!\n";
				datifier(chiaReader());
				cout<<" since last watered!\n";
			}
			//adds water time to chia log
			
		}
		else{cout<<"You need to seed your pet first!\n";}
		return false;
	}
	
	else if (action == "clean"){
		cout<<"You cleaned your chia pet!\n";
		if( _access( "chialog.txt", 00 ) != -1 ){
			remove("chialog.txt");
		}
		return false;
	}
	
	else if (action=="bye"){//||"Bye"||"BYE"||"GOODBYE"||"Goodbye"||"goodbye"){
		return true;
	}
	else{
		if (growthLogger()!=-2&&growthLogger()!=7){
			cout<<"Invalid option. Try saying \"water,\" \"soak,\" \"seed,\" \"clean\" or \"bye\".\n";
			return false;
		}else{
			cout<<"Invalid option. Try saying \"clean\" or \"bye\".\n";
			return false;
		}
	}
}
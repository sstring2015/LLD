#include<iostream>
#include<vector>
#include<chrono>

using namespace std;


class Payment{
    public:
    virtual double calculateCost(double hour)=0;

};

class CarPayment:public Payment{
    public:
    double calculateCost(double hours){
        return hours*3;
    }
};

class BikePayment:public Payment{
    public:
    double calculateCost(double hours){
        return hours*2;
    }
};

class HandicappedPayment:public Payment{
    public:
     double calculateCost(double hours){
        return 0;
    }
};

class Vehicle{
    protected:
    Payment *payment;
    chrono::time_point<chrono::system_clock> parkedTime;

    public:
    virtual string getType()=0;
    virtual double calculateCost(double hours)
    {
        payment->calculateCost(hours);
    }
    void setParkedTime() {
        parkedTime = std::chrono::system_clock::now();
    }

    chrono::time_point<chrono::system_clock> getParkedTime() {
        return parkedTime;
    }  
};

class Car : public Vehicle {
  public:
    Car() 
    {  payment = new CarPayment();
        this->setParkedTime();
    }
    string getType() { return "Car"; }
};

class Bike : public Vehicle {
  public:
    Bike() 
    {
         payment = new BikePayment();
         this->setParkedTime();
    }
    string getType() { return "Bike"; }
};

class HandicappedVehicle : public Vehicle {
  public:
    HandicappedVehicle() 
    {   payment = new HandicappedPayment(); 
        this->setParkedTime();
    }
    string getType() { return "Handicapped"; }
};


class ParkingLot{
    private:
    vector<vector<vector<pair<Vehicle*,bool>>>>spots;
    //here bool respresents if that spot is avilable or not
    int floors;
    int rows;
    int spotsPerRow;

    public:
    ParkingLot(int floors,int rows,int spotPerRow)
    {
        this->floors=floors;
        this->rows=rows;
        this->spotsPerRow=spotPerRow;
        spots.resize(floors);
        for(int i=0;i<floors;i++)
        {
             spots[i].resize(rows);
             for(int j=0;j<rows;j++)
             {
                spots[i][j].resize(spotsPerRow);
             }
        }
        for(int i=0;i<floors;i++)
        {
            for(int j=0;j<rows;j++)
            {
                for(int k=0;k<spotPerRow;k++)
                {
                    spots[i][j][j].second=false;
                }
            }
        }

       
    }


    bool park(Vehicle * v)
    {

        vector<int>found_spot=availableSpots();
        if(found_spot.size()==0)
        {
            cout<<" Parking Lot is Full Please Come Later"<<endl;
            return false;
        }
        int available_floor=found_spot[0];
        int available_row=found_spot[1];
        int available_rowspot=found_spot[2];

        spots[available_floor][available_row][available_rowspot] = {v,true};
        cout << v->getType() << " parked successfully at floor " << available_floor << ", row " << available_row << ", spot " << available_rowspot << "." << endl;
        return true;
    }


      bool leave(Vehicle* v) {
        for (int i = 0; i < floors; i++) {
            for (int j = 0; j < rows; j++) {
                for (int k = 0; k < spotsPerRow; k++) {
                    if (spots[i][j][k].first == v){
                        double hours = calculateHoursParked(spots[i][j][k].first);
                        double cost = spots[i][j][k].first->calculateCost(hours);
                        spots[i][j][k].second= false;
                        cout << v->getType() << " left successfully. Total cost: " << cost <<endl;
                        return true;
                    }
                }
            }
        }
        cout << v->getType() << " not found." << endl;
        return false;
    }
    //availableSpot Will return you a vector for floor, row and spot
    
    vector<int> availableSpots()
    {
        for(int i=0;i<floors;i++)
        {
            for(int j=0;j<rows;j++)
            {
                for(int k=0;k<spotsPerRow;k++)
                {
                    if(spots[i][j][k].second==false)
                    return {i,j,k};
                }
            }
        }
        return {};
    }

     int handicappedSpots(int floor) {
        int count = 0;
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < spotsPerRow; j++) {
                if (dynamic_cast<HandicappedVehicle*>(spots[floor][i][j].first) != nullptr) {
                    count++;
                }
            }
        }
        return count;
    }

    double calculateHoursParked(Vehicle* v) {
        for (int i = 0; i < floors; i++) {
            for (int j = 0; j < rows; j++) {
                for (int k = 0; k < spotsPerRow; k++) {
                    if (spots[i][j][k].first == v) {
                        // get the current time
                        auto now = chrono::system_clock::now();
                        // get the time the vehicle was parked
                        auto parkedTime = spots[i][j][k].first->getParkedTime();
                        // calculate the difference in hours
                        auto duration = chrono::duration_cast<chrono::hours>(now - parkedTime);
                        return duration.count();
                    }
                }
            }
        }
        return 0;
    }
};



int main()
{

    ParkingLot lot(3, 1, 1);
    Car car1, car2;
    Bike bike1, bike2;
    HandicappedVehicle hv1;
    lot.park(&car1);
    lot.park(&car2);
    lot.park(&bike1);
    lot.park(&bike2);
    lot.leave(&car1);
    lot.park(&hv1);
    lot.leave(&bike2);


     return 0;





}







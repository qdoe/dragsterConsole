#include <iostream>
#include <iomanip>
#include <fstream>
#include <bitset>
#include <vector>
#include <string>
#include <cmath>
#include <Windows.h>
#include "TeensyReader.h"

#define UP 0
#define DOWN 1
#define LEFT 2
#define RIGHT 3
#define BUTTON 4

struct dragster {
    int tach = 0;
    int speed = 0;
    int distance = 0;
    int gear = 0;
    int ptach = 0;

    int frame = -79; //might be wrong

    int framerule;

    bool clutch = false;
    
    bool blown = false;
    bool early = false;
    bool done = false;

    dragster(int i) {
        framerule = i;
    };

    bool adv() {
        int x = 1;
        for (int i = 1; i < gear; i++) x *= 2;
        return !((frame + framerule) % x);
    }

    int lim() {
        int p = 1;
        for (int i = 0; i < gear - 1; i++) p *= 2;
        int o = tach * p;
        if (tach < 20) return o;
        return o + p / 2;
    }

    friend std::ostream& operator<<(std::ostream& os, const dragster& a) {
        if (a.blown) {
            os << "BLOWN";
            return os;
        }
        if (a.early) {
            os << "EARLY" << 0.0334f * a.frame;
            return os;
        }
        if (!a.done) {
            os << "DNF";
            return os;
        }
        os << 0.0334f * a.frame;
        return os;
    }

    float rst() {
        float out = 1000.0f;
        if (tach && !done) out = 1111.0f;
        if (done && !blown) out = 0.0334f * frame;
        tach = 0;
        speed = 0;
        distance = 0;
        gear = 0;
        ptach = 0;
        frame = -79;
        clutch = false;
        blown = false;
        early = false;
        done = false;
        return out;
    }

    void p() {
        if(!(frame%10)) std::cout << "adv" << "\t" << "frame" << "\t" << "gear" << "\t" << "spd" << "\t" << "lim" << "\t" << "tach" << "\t" << "ptach\t" << "dist" << std::endl;
        
        std::cout << adv() << "\t" << frame << "\t" << gear << "\t" << speed << "\t" << lim() << "\t" << tach << "\t" << ptach << "\t" << distance;
    }
};

/*
distance depends on distance' and speed'
adv depends on gear and frame
tach depends on shift', gas*, posttach', adv
limit depends on tach and gear
posttach depends on shift', tach, limit, speed'
speed depends on gear, shift', limit, speed'
*/
bool next(dragster& a, std::bitset<5> i) {
    if (a.done) return true;

    a.frame++;

    //distance
    a.distance += a.speed;
    if (a.distance >= 97 * 256) {
        a.done = true;
        return true;
    }

    //gear
    if (a.clutch && !i[LEFT] && a.gear < 4) {
        a.gear++;
        if (a.frame < 0) {
            a.early = true;
            return true;
        }
    }

    //tach
    a.tach = a.ptach;
    if (a.clutch || a.gear == 0) {
        a.tach += ((i[BUTTON]) ? 3 : -3);
    } else if (a.adv()) {
        a.tach += ((i[BUTTON]) ? 1 : -1);
    }
    if (a.tach < 0) a.tach = 0;
    if (a.tach > 31) {
        a.blown = true;
        a.done = true;
        a.tach = 0;
        return true;
    }

    a.ptach = a.tach;
    if(!a.clutch&& a.lim() - a.speed >= 16 && a.gear) a.ptach--;

    //speed
    if (a.gear > 0 && !a.clutch) {
        if (a.speed < a.lim()) {
            a.speed += 2;
        } else if (a.speed > a.lim()) {
            a.speed--;
        }
    }

    a.clutch = i[LEFT];
    return false;
}

std::vector<std::vector<dragster>> init() {
    std::vector<std::vector<dragster>> o;
    for (int j = 0; j < 2; j++) {
        std::vector<dragster> x;
        for (int i = 0; i < 8; i++) x.push_back(dragster(i));
        o.push_back(x);
    }
    return o;
}

void drags()
{
    std::string l;
    std::ifstream f("inputs.txt");

    std::vector<std::bitset<5>> inputs;
    
    while (std::getline(f, l)) {
        inputs.push_back(std::bitset<5>(std::stoi(l)));
    }
    std::vector<dragster> x;
    for (int i = 0; i < 8; i++) x.push_back(dragster(i));

    for (auto t : inputs) {
        for(int i = 0; i < x.size(); i++){
            next(x[i], t);
        }
    }

    for (auto& g : x) {
        std::cout << g << std::endl;
    }
}

int main() {
    SetConsoleTitle(TEXT("dragster"));
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    TeensyReader x;

    int attempts = 0;
    bool newline = false;

    std::vector<std::vector<dragster>> y = init();
    bool even = false;
    std::vector<bool> rst = { false,false };
    std::vector<int> poss = []() {
        std::vector<int> o;
        for (int i = 0; i < 14; i++) o.push_back(i);
        return o;
    }();

    auto acc = [&y](int i) {
        return y[i % 2][i / 2];
    };
    y = init();

    while (true) {
        std::bitset<5> in = x.read();

        if (rst[even] && !in[RIGHT]) {
            float p = 1111.0f;
            for (int i = 0; i < 8; i++) {
                p = std::fmin(p,y[even][i].rst());
            }
            int k = 15;
            if (p < 5.60f) {
                k = 0xa1;
            }
            else if (p < 5.70f) {
                k = 0xb2;
            }
            SetConsoleTextAttribute(hConsole, k);
            std::cout << p;
            SetConsoleTextAttribute(hConsole, 15);
            std::cout << "\t";
            if(newline) std::cout << ++attempts << std::endl;
            newline = !newline;
            rst[even] = false;
        }
        else if (in[RIGHT]) {
            rst[even] = true;
        }
        for (int i = 0; i < 8; i++) {
            next(y[even][i], in);
        }
        even = !even;
    }

    return 0;
}
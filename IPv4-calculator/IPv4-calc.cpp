/*
    IP-calc for IPv4 addresses.
Usage:
    1) Address: x.x.x.x
    2) Subnet:  x.x.x.x
    
By Matthew Benjamin (spybull)
*/

#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <cmath>
#include <iomanip>

using namespace std;

int BinToChar(int);
int ShowBinaryAddress(vector<int>&);
void PushIpNumeric(vector<int>&, string);
void ShowDecAddress(vector<int>&, string);
vector<int> GetHostMax(vector<int>&, vector<int>&);


enum
{
    ADDRESS,
    SUBNET,
    NETWORK,
    WILDCARD,
    BROADCAST,
    HOSTMIN,
    HOSTMAX
};


int main()
{
    const int N = 7;
    vector<int> vIP[N];
    string address, subnet;
    string sIP[] = {"Address", "Subnet", "Network", "Wildcard", "Broadcast", "Hostmin", "HostMax"};
    int hostMax = 0, i = 0;

//    cout << "Enter IPv4 address: ";
    cin >> address;
    
//    cout << "Enter subnet mask: ";
    cin >> subnet;


    PushIpNumeric(vIP[ADDRESS], address);
    PushIpNumeric(vIP[SUBNET], subnet);

    for (i = 0; i < 4; i++)
    {
        vIP[NETWORK].push_back((vIP[ADDRESS][i] & vIP[SUBNET][i]));
        vIP[WILDCARD].push_back((vIP[SUBNET][i] ^ 255));
        vIP[BROADCAST].push_back(vIP[ADDRESS][i] | vIP[WILDCARD][i]);
    }
        
    vIP[HOSTMAX] = GetHostMax(vIP[NETWORK], vIP[BROADCAST]);
    for (int i = 0; i < N; i++)
    {
        if (i == HOSTMIN)
        {
            vIP[NETWORK][3] += 1;
            ShowDecAddress(vIP[NETWORK], sIP[HOSTMIN]);
            ShowBinaryAddress(vIP[NETWORK]);
            vIP[NETWORK][3] -= 1;
            continue;
        }

        ShowDecAddress(vIP[i], sIP[i]);
        if(i == SUBNET)
            hostMax = ShowBinaryAddress(vIP[SUBNET]);
        else
            ShowBinaryAddress(vIP[i]);
    }
    

    cout << setw(15) << "Hosts: " << pow(2, hostMax) - 2 << endl;

    return 0;
}

int ShowBinaryAddress(vector<int> &vArr)
{
    int z = 0;
    for (auto i : vArr)
        z += BinToChar(i);
    cout << endl;

    return z;
}

vector<int> GetHostMax(vector<int> &net, vector<int> &broad)
{
    vector<int> vTemp;
    for (int i = 0; i < 4; i++)
        vTemp.push_back(net[i] | broad[i]);

    vTemp[3] -= 1;
    return vTemp;
}

void ShowDecAddress(vector<int> &vArr, string out)
{
    ostringstream oss;
    copy(vArr.begin(), vArr.end() - 1, ostream_iterator<int>(oss, "."));
    oss << vArr.back();
    cout << left << setw(15) << out << setw(20) << oss.str();
}

void PushIpNumeric(vector<int> &num, string IPv4)
{
    const int N = 4;
    int last = 0, next = 0;
    int i = 0;

    while (i < N)
    {
        next = IPv4.find('.', last);
        string number = IPv4.substr(last, next - last);
        num.push_back(atoi(number.c_str()));
        i++;
        last = next + 1;
    }
}

int BinToChar(int isub)
{
    int mask{ 1 << (sizeof(char) * 7) };
    int zero = 0;

    for (auto n = 0; n < 8; n++)
    {
        if (isub & mask)
        {
            cout << '1';
        }
        else
        {
            cout << '0'; zero++;
        }
        mask >>= 1;
    }

    cout << ' ';
    return zero;
}

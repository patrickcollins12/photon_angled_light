#include <iostream>
using namespace std;

struct product {
  int weight;
  double price;
} ;

product apple;
product banana, melon;

int main()
{
apple.weight = 10;
cout << "Hello, World!" << apple.weight << endl;
return 0;
}

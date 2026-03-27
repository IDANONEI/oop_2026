#include <iostream>
#include <format>
#include <vector>

using namespace std;

template<class T>
concept Sumable = requires(T t)
{
	t + t;
};

template<Sumable T>
T Sum(const T& a, const T& b)
{
	return a + b;
}

int main()
{
	cout << Sum(1, 2) << endl; // 3
	cout << Sum(1.5, 2.2) << endl; // 3
	cout << Sum(std::string("hello"), std::string("world")) << endl; // 3

	//std::vector<int> array1 = { 1,2,3 };
	//std::vector<int> array2 = { 4,5,6 };
	//std::vector<int> array3 = Sum(array1, array2);

	return 0;
}


// template<class T>
// class optional
// {
// 	bool has_value;
// 	T value;
// };

//static_assert(!std::is_reference_v<T>);
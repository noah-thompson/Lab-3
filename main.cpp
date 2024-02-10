#include <iostream>
#include <sstream>
#include <string>

#include "socklib.h"

std::string do_client(std::istream &in_stream);
int run_test();
std::string build_string(std::istream& in_stream);

int main(int argc, char *argv[]) {
 
	SockLibInit();
	return run_test();
}

std::string do_client(std::istream &in_stream) {

	Socket socket(Socket::Family::INET, Socket::Type::STREAM);

	std::string str_address("68.183.63.165"); // <- Use IP address, not MAC address
	Address address(str_address);

	socket.Connect(address, 7778);

	// Send data (this is the same interface as file.write())
	std::string str = build_string(in_stream);


	return str;
}

std::string build_string(std::istream &in_stream) {
  
	std::string str = "SORTED";
	std::string temp; 
	std::vector<float> nums;
	bool isNum = true;
	std::stringstream ss;

	do
	{
		std::getline(in_stream, temp);
		if (temp == "done")
		{
			break;
		}
		for (int i = 0; i < temp.length(); i++)
		{
			if (!isdigit(temp[i]) && temp[i] != '-' && temp[i] != '.')
			{
				isNum = false;
			}
		}
		if (isNum)
		{
			nums.push_back(stof(temp));
		}

		isNum = true;

	} while (!in_stream.eof());

	for (int i = 0; i < nums.size(); i++)
	{
		for (int j = i+1; j < nums.size(); j++)
		{
			if (std::to_string(nums[i])[0] >= std::to_string(nums[j])[0])
			{
				std::swap(nums[i], nums[j]);
			}
		}
		if (nums[i] == int(nums[i]))
		{

			str += " " + std::to_string(int(nums[i]));
		}
		else
		{
			nums[i] = round(nums[i] * 10.)/10.;
			ss << nums[i];
			ss >> temp;
			str += " " + temp;
			ss.clear();
			ss.str("");
		}
	}

	return str;
}

int run_test() {
  std::stringstream input("30\n500\n-12\n3.6\n"
			  "not a number\n200also not a number\n"
			  "88.1\ndone");

  std::cout << "Testing on input '" << input.str() << "'.\n";

  std::string msg = do_client(input);
  std::string expected("SORTED -12 3.6 30 500 88.1");
  std::cout << "do_client() returned '" << msg << "'.\n";

  if (msg != expected) {
    std::cout << "TEST FAILED. Expected '" << expected << "' but was '" << msg
              << "'.\n";
    return 1;
  }
  
  std::cout << "Recieved expected message '" << msg << "'. Test passed 😄\n";
  return 0;
}

#include <iostream>
#include <fstream>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;


void Binarization_manual(Mat gray, Mat& binary);
int Pixel_Counting(Mat binary, int& num_black, int& num_white);
std::vector<int> ConnectedArea_Analysis(Mat binary);
void ConnectedArea_Output(std::vector<int> pixel_num);
void Figure_Output(Mat binary);



int main()
{
	// Read the original image
	std::cout << "Please input the figure path" << std::endl;
	std::cout << ">>>";
	std::string path;
	std::cin >> path;
	std::cin.clear();
	std::cin.ignore();
	std::cout << std::endl;
	Mat img = imread(path);
	Mat gray,binary;
	std::vector<int> pixel_num;
	int num_black, num_white, flag;
	

	// Convert to grayscale
	cvtColor(img, gray, COLOR_BGR2GRAY);

	// Convert to binary
	Binarization_manual(gray, binary);

	bool isContinue = true;
	while (isContinue)
	{
		std::cin.clear();
		std::cin.ignore();
		int function_label;
		std::cout << "Please input the function label" << std::endl;
		std::cout << "1 : Show the number of black and white pixels" << std::endl;
		std::cout << "2 : Show the distribution of black areas" << std::endl;
		std::cout << "3 : Output the distribution of black areas to file" << std::endl;
		std::cout << "4 : Output the binary figure to file" << std::endl;
		std::cout << "5 : Quit" << std::endl;
		std::cin >> function_label;
		switch (function_label)
		{
		case 1:
			num_black = 0;
			num_white = 0;
			flag = Pixel_Counting(binary, num_black, num_white);
			if (flag == 0)
			{
				std::cout << std::endl;
				std::cout << ">>>" << std::endl;
				std::cout << "Black : " << num_black << std::endl;
				std::cout << "White : " << num_white << std::endl;
				std::cout << "Total : " << (binary.cols * binary.rows) << std::endl;
				std::cout << "Black fraction : " << (1.0 * num_black / (binary.cols * binary.rows)) << std::endl;
				std::cout << "White fraction : " << (1.0 * num_white / (binary.cols * binary.rows)) << std::endl;
				std::cout << ">>>" << std::endl;
				std::cout << std::endl;
			}
			waitKey(0);
			break;

		case 2:
			pixel_num = ConnectedArea_Analysis(binary);
			std::cout << std::endl;
			std::cout << ">>>" << std::endl;
			for (int i = 0; i < pixel_num.size(); i++)
			{
				std::cout << "i : " << pixel_num.at(i) << std::endl;
			}
			std::cout << ">>>" << std::endl;
			std::cout << std::endl;
			break;

		case 3:
			pixel_num = ConnectedArea_Analysis(binary);
			ConnectedArea_Output(pixel_num);
			std::cout << std::endl;
			std::cout << "Output complete" << std::endl;
			std::cout << std::endl;
			break;

		case 4:
			Figure_Output(binary);
			std::cout << std::endl;
			std::cout << "Output complete" << std::endl;
			std::cout << std::endl;
			break;

		case 5:
			isContinue = false;
			break;
		default:
			std::cout << "Input ERROR" << std::endl;
			std::cin.clear();
			std::cin.ignore();
			break;
		}
	}
	
	return 0;
}


void Binarization_manual(Mat gray, Mat &binary)
{
	int thre;
	while (true)
	{
		std::cout << "Please input the threshold" << std::endl;
		std::cout << ">>>";
		std::cin >> thre;
		std::cin.clear();
		std::cin.ignore();
		std::cout << std::endl;
		if (thre < 0 || thre > 255)
		{
			break;
		}
		threshold(gray, binary, thre, 255, THRESH_BINARY);
		namedWindow("Binary", WINDOW_NORMAL);
		imshow("Binary", binary);
		while (waitKey(1) != ' ');
		destroyAllWindows();
	}
	
}

int Pixel_Counting(Mat binary, int& num_black, int& num_white)
{
	int val;
	num_black = 0, num_white = 0;
	for (int i = 0; i < binary.rows; i++)
	{
		for (int j = 0; j < binary.cols; j++)
		{
			val = (int)(binary.at<uchar>(i, j));
			if (val < 0 || val > 256)
			{
				return 1;
			}
			if (val == 0)
			{
				num_black++;
			}
			else
			{
				num_white++;
			}
		}
	}
	return 0;
}

std::vector<int> ConnectedArea_Analysis(Mat binary)
{
	int num;
	Mat label, inverse;
	bitwise_not(binary, inverse);
	num = connectedComponents(inverse, label, 8, CV_32S);
	std::vector<int> pixel_num(num + 1, 0);
	for (int i = 0; i < label.rows; i++)
	{
		for (int j = 0; j < label.cols; j++)
		{
			pixel_num.at(label.at<int>(i, j)) += 1;
		}
		
	}
	return pixel_num;
}

void ConnectedArea_Output(std::vector<int> pixel_num)
{
	std::cout << "Please input the data output path" << std::endl;
	std::cout << ">>>";
	std::string path;
	std::cin >> path;
	std::cin.clear();
	std::cin.ignore();
	std::ofstream file_out(path);
	for (int i = 0; i < pixel_num.size(); i++)
	{
		file_out << i << " : " << pixel_num.at(i) << std::endl;
	}
	file_out.close();
}

void Figure_Output(Mat binary)
{
	std::string output_path;
	std::cout << "Please input the figure output path" << std::endl;
	std::cout << ">>>";
	std::cin >> output_path;
	std::cin.clear();
	std::cin.ignore();
	imwrite(output_path, binary);
}


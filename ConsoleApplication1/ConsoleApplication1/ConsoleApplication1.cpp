// ConsoleApplication1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <dlib/gui_widgets.h>
#include <dlib/image_io.h>
#include <dlib/image_transforms.h>
#include <fstream>


using namespace std;
using namespace dlib;

//  ----------------------------------------------------------------------------

int main(int argc, char** argv)
{
	try
	{

		// 声明图像
		array2d<rgb_pixel> img;

		string img_path = "lena.jpg";
		load_image(img, img_path);

		// 高斯模糊
		array2d<unsigned char> blurred_img;
		gaussian_blur(img, blurred_img);

		// sobel边缘检测
		array2d<short> horz_gradient, vert_gradient;
		array2d<unsigned char> edge_image;
		sobel_edge_detector(blurred_img, horz_gradient, vert_gradient);

		//非极大边缘抑制
		suppress_non_maximum_edges(horz_gradient, vert_gradient, edge_image);

		// 显示结果
		image_window my_window(edge_image, "Normal Edge Image");

		// We can also easily display the edge_image as a heatmap or using the jet color
		// scheme like so.
		image_window win_hot(heatmap(edge_image));
		image_window win_jet(jet(edge_image));

		// also make a window to display the original image
		image_window my_window2(img, "Original Image");

		// Sometimes you want to get input from the user about which pixels are important
		// for some task.  You can do this easily by trapping user clicks as shown below.
		// This loop executes every time the user double clicks on some image pixel and it
		// will terminate once the user closes the window.
		point p;
		while (my_window.get_next_double_click(p))
		{
			cout << "User double clicked on pixel:         " << p << endl;
			cout << "edge pixel value at this location is: " << (int)edge_image[p.y()][p.x()] << endl;
		}

		// wait until the user closes the windows before we let the program 
		// terminate.
		win_hot.wait_until_closed();
		my_window2.wait_until_closed();


		// Finally, note that you can access the elements of an image using the normal [row][column]
		// operator like so:
		cout << horz_gradient[0][3] << endl;
		cout << "number of rows in image:    " << horz_gradient.nr() << endl;
		cout << "number of columns in image: " << horz_gradient.nc() << endl;
	}
	catch (exception& e)
	{
		cout << "exception thrown: " << e.what() << endl;
	}
	return 0;
}


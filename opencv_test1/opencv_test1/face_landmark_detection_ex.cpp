// The contents of this file are in the public domain. See LICENSE_FOR_EXAMPLE_PROGRAMS.txt
/*

    This example program shows how to find frontal human faces in an image and
    estimate their pose.  The pose takes the form of 68 landmarks.  These are
    points on the face such as the corners of the mouth, along the eyebrows, on
    the eyes, and so forth.  
    


    The face detector we use is made using the classic Histogram of Oriented
    Gradients (HOG) feature combined with a linear classifier, an image pyramid,
    and sliding window detection scheme.  The pose estimator was created by
    using dlib's implementation of the paper:
       One Millisecond Face Alignment with an Ensemble of Regression Trees by
       Vahid Kazemi and Josephine Sullivan, CVPR 2014
    and was trained on the iBUG 300-W face landmark dataset (see
    https://ibug.doc.ic.ac.uk/resources/facial-point-annotations/):  
       C. Sagonas, E. Antonakos, G, Tzimiropoulos, S. Zafeiriou, M. Pantic. 
       300 faces In-the-wild challenge: Database and results. 
       Image and Vision Computing (IMAVIS), Special Issue on Facial Landmark Localisation "In-The-Wild". 2016.
    You can get the trained model file from:
    http://dlib.net/files/shape_predictor_68_face_landmarks.dat.bz2.
    Note that the license for the iBUG 300-W dataset excludes commercial use.
    So you should contact Imperial College London to find out if it's OK for
    you to use this model file in a commercial product.


    Also, note that you can train your own models using dlib's machine learning
    tools.  See train_shape_predictor_ex.cpp to see an example.

    


    Finally, note that the face detector is fastest when compiled with at least
    SSE2 instructions enabled.  So if you are using a PC with an Intel or AMD
    chip then you should enable at least SSE2 instructions.  If you are using
    cmake to compile this program you can enable them by using one of the
    following commands when you create the build project:
        cmake path_to_dlib_root/examples -DUSE_SSE2_INSTRUCTIONS=ON
        cmake path_to_dlib_root/examples -DUSE_SSE4_INSTRUCTIONS=ON
        cmake path_to_dlib_root/examples -DUSE_AVX_INSTRUCTIONS=ON
    This will set the appropriate compiler options for GCC, clang, Visual
    Studio, or the Intel compiler.  If you are using another compiler then you
    need to consult your compiler's manual to determine how to enable these
    instructions.  Note that AVX is the fastest but requires a CPU from at least
    2011.  SSE4 is the next fastest and is supported by most current machines.  
*/

#include "stdafx.h"
#define INTMAX_MAX   INT64_MAX
#include "dlib/image_processing/frontal_face_detector.h"
#include "dlib/image_processing/render_face_detections.h"
#include "dlib/image_processing.h"
#include "dlib/gui_widgets.h"
#include "dlib/image_io.h"
#include <iostream>

using namespace std;
using namespace dlib;
// ----------------------------------------------------------------------------------------


int main(int argc, char** argv)
{
	try
	{

		frontal_face_detector detector = get_frontal_face_detector();
		image_window win;

		// Loop over all the images provided on the command line.
		for (int i = 1; i < argc; ++i)
		{
			
			array2d<unsigned char> img;
			return 0;
			load_image(img, "d:\\opencv_leane\\image\\2.jpg");
			// Make the image bigger by a factor of two.  This is useful since
			// the face detector looks for faces that are about 80 by 80 pixels
			// or larger.  Therefore, if you want to find faces that are smaller
			// than that then you need to upsample the image as we do here by
			// calling pyramid_up().  So this will allow it to detect faces that
			// are at least 40 by 40 pixels in size.  We could call pyramid_up()
			// again to find even smaller faces, but note that every time we
			// upsample the image we make the detector run slower since it must
			// process a larger image.
			pyramid_up(img);

			// Now tell the face detector to give us a list of bounding boxes
			// around all the faces it can find in the image.
			std::vector<rectangle> dets = detector(img);

			cout << "Number of faces detected: " << dets.size() << endl;
			// Now we show the image on the screen and the face detections as
			// red overlay boxes.
			win.clear_overlay();
			win.set_image(img);
			win.add_overlay(dets, rgb_pixel(255, 0, 0));

			cout << "Hit enter to process the next image..." << endl;
			cin.get();
		}
		imshow("ss",win)
	}
	catch (exception& e)
	{
		cout << "\nexception thrown!" << endl;
		cout << e.what() << endl;
	}
}
// ----------------------------------------------------------------------------------------


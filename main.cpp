/*
 * The Example of the Withrobot oCam-1MGN-U API using with OpenCV. (Linux only.)
 *
 * This example shows how to get image from the oCam-1MGN using the Withrobot camera API.
 * And also shows how to control the oCam-1MGN using the Withrobot camera API.
 *
 * This example program usage.:
 * 	- Press the key 'q' for exit this program.
 *  - Press the key ']' for increase the exposure.
 *  - Press the key '[' for decrease the exposure.
 *  - Press the key '=' for increase the brightness.
 *  - Press the key '-' for decrease the brightness.
 */

#include <stdio.h>
#include <errno.h>
#include <iostream>

#include <unistd.h> // usleep

//#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>

#include <opencv2/imgproc.hpp>
#include <opencv2/aruco.hpp>  

#include "withrobot_camera.hpp"	/* withrobot camera API */

/*
 *	Main
 */
int main (int argc, char* argv[])
{

  
  cv::Mat markerImage;
  cv::Ptr<cv::aruco::Dictionary> dictionary = cv::aruco::getPredefinedDictionary(cv::aruco::DICT_6X6_250);
  cv::aruco::drawMarker(dictionary, 23, 200, markerImage, 1);
  cv::namedWindow("aruco", cv::WINDOW_AUTOSIZE|cv::WINDOW_KEEPRATIO);
  cv::imshow("aruco", markerImage);

  std::ios::sync_with_stdio(false);
    //std::cout << "Beginning basic camera application\n";
    /*
     * initialize oCam-1MGN
     *
     * [ supported image formats ]
     *
     * USB 3.0
     * 	[0] "8-bit Greyscale 1280 x 720 60 fps"
	 *	[1] "8-bit Greyscale 1280 x 960 45 fps"
	 *	[2] "8-bit Greyscale 320 x 240 160 fps"
	 * 	[3] "8-bit Greyscale 640 x 480 80 fps"
	 *
	 * USB 2.0
     * 	[0] "8-bit Greyscale 1280 x 720 30 fps"
	 *	[1] "8-bit Greyscale 1280 x 960 22.5 fps"
	 *	[2] "8-bit Greyscale 320 x 240 160 fps"
	 * 	[3] "8-bit Greyscale 640 x 480 80 fps"
	 *
     */
	const char* devPath = "/dev/video0";
  Withrobot::Camera camera(devPath);

    std::vector<const char*> formatList, resolutionList, ratioList; 
    camera.get_valid_image_format_list(formatList);
    for (unsigned int i=0;i<formatList.size();i++) {
      camera.get_valid_resolution_list(formatList[i], resolutionList);
      for (unsigned int j=0; j<resolutionList.size();j++) {
        camera.get_valid_ratio_list(resolutionList[j], ratioList);
        for (unsigned int k=0; k<ratioList.size(); k++) {
          //std::cout << resolutionList[j] << ": " << ratioList[k] << std::endl;
        }
      }
    }



    /* USB 3.0 */
    /* 8-bit Greyscale 1280 x 720 60 fps */
   // camera.set_format(1280, 720, Withrobot::fourcc_to_pixformat('G','R','E','Y'), 1, 60);

    /* 8-bit Greyscale 1280 x 960 45 fps */
//    camera.set_format(1280, 960, Withrobot::fourcc_to_pixformat('G','R','E','Y'), 1, 45);
    camera.set_format(1280, 960, Withrobot::fourcc_to_pixformat('G','R','E','Y'), 1, 30);

    /* 8-bit Greyscale 320 x 240 160 fps */
    //camera.set_format(320, 240, Withrobot::fourcc_to_pixformat('G','R','E','Y'), 1, 160);

    /* 8-bit Greyscale 640 x 480 80 fps */
    //camera.set_format(640, 480, Withrobot::fourcc_to_pixformat('G','R','E','Y'), 1, 80);

    /* USB 2.0 */
    /* 8-bit Greyscale 1280 x 720 30 fps */
//    camera.set_format(1280, 720, Withrobot::fourcc_to_pixformat('G','R','E','Y'), 1, 30);

    /* 8-bit Greyscale 1280 x 960 22.5 fps */
    //camera.set_format(1280, 960, Withrobot::fourcc_to_pixformat('G','R','E','Y'), 2, 45);

    /* 8-bit Greyscale 320 x 240 160 fps */
    //camera.set_format(320, 240, Withrobot::fourcc_to_pixformat('G','R','E','Y'), 1, 160);

    /* 8-bit Greyscale 640 x 480 80 fps */
    //camera.set_format(640, 480, Withrobot::fourcc_to_pixformat('G','R','E','Y'), 1, 80);

    /*
     * get current camera format (image size and frame rate)
     */
    Withrobot::camera_format camFormat;
    camera.get_current_format(camFormat);

    /*
     * Print infomations
     */
    std::string camName = camera.get_dev_name();
    std::string camSerialNumber = camera.get_serial_number();

    //printf("dev: %s, serial number: %s\n", camName.c_str(), camSerialNumber.c_str());
    //printf("----------------- Current format informations -----------------\n");
    //camFormat.print();
    //printf("---------------------------------------------------------------\n");

    /*
     * [ supported camera controls; The double quotes are the 'get_control' and the 'set_control' function string argument values. ]
     *
     *  [0] "Brightness",          Value(default [min, step, max]): 64 ( 64 [0, 1, 127] )  // gain
     *  [1] "Exposure (Absolute)", Value(default [min, step, max]): 39 ( 39 [1, 1, 625] )
     *
     */
    int brightness = camera.get_control("Brightness");
    int exposure = camera.get_control("Exposure (Absolute)");
    //std::cout << "Brightness: " << brightness << std::endl;
    //std::cout << "Exposure: " << exposure << std::endl;

    camera.set_control("Brightness", brightness);
    camera.set_control("Exposure (Absolute)", exposure);

    /*
     * Start streaming
     */
    if (!camera.start()) {
    	perror("Failed to start.");
    	exit(0);
    }

    /*
     * Initialize OpenCV
     */
    std::string windowName = camName + " " + camSerialNumber;
    cv::Mat srcImg(cv::Size(camFormat.width, camFormat.height), CV_8UC1);
    cv::namedWindow(windowName.c_str(), cv::WINDOW_AUTOSIZE|cv::WINDOW_KEEPRATIO);

    /*
     * Main loop
     */
    bool quit = false;
    while (!quit) {
    	/* Copy a single frame(image) from camera(oCam-1MGN). This is a blocking function. */
    	int size = camera.get_frame(srcImg.data, camFormat.image_size, 1);

    	/* If the error occured, restart the camera. */
    	if (size == -1) {
    	    //("error number: %d\n", errno);
    	    //perror("Cannot get image from camera");
    	    camera.stop();
    	    camera.start();
    	    continue;
    	}
    	/* Show image */
      int erodeSz = 4;
      cv::Mat erodeElement  = getStructuringElement(cv::MORPH_RECT, cv::Size(2*erodeSz+1,2*erodeSz+1));
      cv::erode(srcImg, srcImg, erodeElement);
      cv::dilate(srcImg,srcImg,erodeElement);

    	cv::imshow(windowName.c_str(), srcImg);
    	char key = cv::waitKey(10);
    /* OUTPUT TO TERMINAL
      //std::cout << srcImg << std::endl;
      for(size_t i = 0; i < srcImg.dataend-srcImg.datastart; i++ ){
        std::cout << srcImg.data[i];
      }
      */
    	/* Keyboard options */
    	switch (key) {
    	/* When press the 'q' key then quit. */
    	case 'q':
    		quit = true;
    		break;

    	/* When press the '[' key then decrease the exposure time. */
    	case '[':
    		exposure = camera.get_control("Exposure (Absolute)");
    		camera.set_control("Exposure (Absolute)", --exposure);
    		break;

		/* When press the ']' key then increase the exposure time. */
    	case ']':
    		exposure = camera.get_control("Exposure (Absolute)");
    		camera.set_control("Exposure (Absolute)", ++exposure);
    		break;

		/* When press the '-' key then decrease the brightness. */
    	case '-':
    		exposure = camera.get_control("Brightness");
    		camera.set_control("Brightness", --brightness);
    		break;

		/* When press the '=' key then increase the brightness. */
    	case '=':
    		exposure = camera.get_control("Brightness");
    		camera.set_control("Brightness", ++brightness);
    		break;

    	default:
    		break;
    	}
    }

    cv::destroyAllWindows();

    /*
     * Stop streaming
     */
    camera.stop();

	//printf("Done.\n");

	return 0;
}

/**
  * Quellen für den Code aus: http://dlib.net/face_landmark_detection_ex.cpp.html
  * und http://dlib.net/webcam_face_pose_ex.cpp.html
  *
  * */

#include "facedetandfpl.h"
#include "win.h"
#include "facepointstosample.h"

#include <dlib/image_processing/frontal_face_detector.h>
#include <opencv2/highgui/highgui.hpp>
#include <dlib/image_processing.h>

using namespace std;
using namespace dlib;

FaceDetAndFPL::FaceDetAndFPL():
    sendoscmsg(new Sendoscmsg)
    ,fpts(new facepointstosample)
{    
}

void FaceDetAndFPL::findFacesAndPoints(){
    try
    {
        string pathToSPDat = "../shape_predictor_68_face_landmarks.dat";
         //string pathToSPDat = "C:/Users/Muffinman/Desktop/FEASKonsole/shape_predictor_68_face_landmarks.dat";
        cv::VideoCapture cap(0);
        if (!cap.isOpened())
        {
            cerr << "Unable to connect to camera" << endl;
            //return 1;
        }

        // Load face detection and pose estimation models.
        frontal_face_detector detector = get_frontal_face_detector();
        shape_predictor pose_model;
        deserialize(pathToSPDat) >> pose_model;
        // Grab and process frames until the main window is closed by the user.
        while(!winClass.is_closed()){
            // Grab a frame
            cv::Mat temp;
            cap >> temp;
            // Turn OpenCV's Mat into something dlib can deal with.  Note that this just
            // wraps the Mat object, it doesn't copy anything.  So cimg is only valid as
            // long as temp is valid.  Also don't do anything to temp that would cause it
            // to reallocate the memory which stores the image as that will make cimg
            // contain dangling pointers.  This basically means you shouldn't modify temp
            // while using cimg.
            cv_image<bgr_pixel> cimg(temp);

            // Detect faces
            std::vector<rectangle> faces = detector(cimg);
            // Find the pose of each face.
            std::vector<full_object_detection> shapes;
            for (unsigned long i = 0; i < faces.size(); ++i){
                dlib::full_object_detection shape = pose_model(cimg, faces[i]);

                //cout << "pixel position of first part:  " << shape.part(0) << endl;
                shapes.push_back(shape);
                //cout << "shapes" << shapes[0].part(0)(0) <<endl;
                //cout << "faces.size()" << faces.size() <<endl;

            }
            // Display it all on the screen
            if(faces.size() >0){
                sendFacePoints(shapes);
                winClass.drawImage(cimg);
                winClass.drawFacePoints();
                fpts->cap = cap;
                fpts->playSound();
            }
            if(soundModus->on){
                if(fpts->getBrowL() > 90){
                    soundModus->braueLinks();
                    cout<<"braue Links: " <<fpts->getBrowL() << endl;
                }

                else if (fpts->getBrowR() > 90){
                    soundModus->braueRechts();
                    cout<<"braue Rechts: " <<fpts->getBrowR() << endl;
                }

                else if (fpts->getHeadPosX() > 95){
                    soundModus->kopfoben();
                    cout<<"HeadPosX 95: " <<fpts->getHeadPosX() << endl;
                }

                else if (fpts->getHeadPosX() < 5){
                    soundModus->kopfunten();
                    cout<<"HeadPosX 5: " <<fpts->getHeadPosX()<< endl;
                }

                else if (fpts->getHeadPosY() > 95){
                    soundModus->kopfrechts();
                    cout<<"HeadPosY 95: " <<fpts->getHeadPosY()<< endl;
                }

                 else if (fpts->getHeadPosY() < 5){
                    soundModus->kopflinks();
                    cout<<"HeadPosY 5: " <<fpts->getHeadPosY() << endl;
                }

                else if (fpts->getMouthOpen() > 95){
                    soundModus->mundauf();
                    cout<<"MundAuf 95: " <<fpts->getMouthOpen() << endl;
                }

                else if (fpts->getRotationHead() > 95){
                    soundModus->kopfrotation();
                    cout<<"HeadRotation95: " <<fpts->getRotationHead()<< endl;
                }
            }
        }
    }
    catch(serialization_error& e)
    {
        cout << endl << e.what() << endl;
    }
    catch(exception& e)
    {
        cout << e.what() << endl;
    }
}


FaceDetAndFPL::~FaceDetAndFPL()
{
}


void FaceDetAndFPL::sendFacePoints(std::vector<full_object_detection> shapes){
    winClass.shapes = shapes;
    fpts->shapes = shapes;
}

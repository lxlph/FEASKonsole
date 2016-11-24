#include "win.h"
#include <dlib/gui_widgets.h>

Win::Win()
    :imageWin(new dlib::image_window)
{
    //imageWin->show();

}

Win::~Win(){
    //nicht ausführen, sonst schließt sich das Fenster
    //delete imageWin;
}

boolean Win::is_closed(){
    return imageWin->is_closed();
}


void Win::drawImage(dlib::cv_image<dlib::bgr_pixel> cimg){
    imageWin->clear_overlay();
    imageWin->set_image(cimg);
}

void Win::drawFacePoints(){
    imageWin->add_overlay(dlib::render_face_detections(shapes));
}


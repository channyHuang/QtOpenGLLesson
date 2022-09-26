#include "camera.h"

Camera::Camera() : vposition_(QVector3D(0, 0, -1)), vtarget_(QVector3D(0, 0, 0)), vupdir_(QVector3D(0, 1, 0)) {
    mviewmatrix_.lookAt(vposition_, vtarget_, vupdir_);
}

Camera::~Camera() {}

QMatrix4x4 Camera::getViewMatrix() {
    return mviewmatrix_;
}

void Camera::moveCamera(QKeyEvent *e) {
    QVector3D vdir = (vposition_ - vtarget_);
    vdir.normalize();
    switch(e->key()) {
    case Qt::Key_W:
        vtarget_ += vdir;
        vposition_ += vdir;
        break;
    case Qt::Key_S:
        vtarget_ -= vdir;
        vposition_ -= vdir;
        break;
    case Qt::Key_A:
        vdir = QVector3D(vdir.z(), vdir.y(), vdir.x());
        vtarget_ += vdir;
        vposition_ += vdir;
        break;
    case Qt::Key_D:
        vdir = QVector3D(vdir.z(), vdir.y(), vdir.x());
        vtarget_ -= vdir;
        vposition_ -= vdir;
        break;
    case Qt::Key_Q:
        vposition_ += vupdir_;
        break;
    case Qt::Key_E:
        vposition_ -= vupdir_;
        break;
    default:
        break;
    }
    mviewmatrix_.setToIdentity();
    mviewmatrix_.lookAt(vposition_, vtarget_, vupdir_);
}

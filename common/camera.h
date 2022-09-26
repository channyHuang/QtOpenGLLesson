#ifndef CAMERA_H
#define CAMERA_H

#include <QKeyEvent>
#include <QVector3D>
#include <QMatrix4x4>

class Camera {
public:
    Camera();
    virtual ~Camera();

    QMatrix4x4 getViewMatrix();

    void moveCamera(QKeyEvent *e);
private:
    QVector3D vposition_, vtarget_, vupdir_;
    QMatrix4x4 mviewmatrix_;
};

#endif

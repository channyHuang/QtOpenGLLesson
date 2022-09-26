#include "regularfun.h"

namespace Common {

QVector3D getColor(int index) {
    QVector3D colors[] = {QVector3D(1, 0, 0), QVector3D(0, 1, 0), QVector3D(0, 0, 1),
                          QVector3D(1, 1, 0), QVector3D(1, 0, 1), QVector3D(0, 1, 1)};
    if (index >= 0 && index < 6) return colors[index];
    return colors[rand() % 6];
}

}

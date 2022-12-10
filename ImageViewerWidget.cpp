#include "ImageViewerWidget.h"
#include <QScrollBar>

ImageViewer::ImageViewer(QWidget *parent)
    : QGraphicsView{parent},
    m_isPan(false),
    m_prevPan(0,0),
    scene(nullptr)
{
    scene = new QGraphicsScene(this);
    this->setScene(scene);

    setDragMode(QGraphicsView::DragMode::NoDrag);
    setInteractive(false);
    setEnabled(false);
}

void ImageViewer::setQImage(QImage image)
{   if(image.isNull())
        return;
    m_image = QPixmap::fromImage(image);
    initShow();
}

void ImageViewer::setPixmap(QPixmap pixmap)
{
    m_image = pixmap.copy();
    initShow();
}
void ImageViewer::resetImage()
{
    if(m_image.isNull())
        return;
    scene->clear();
    setEnabled(false);
}
QImage  ImageViewer::getQImage()
{
    return m_image.toImage();
}


QPixmap ImageViewer::getPixmap()
{
    return m_image;
}
void ImageViewer::initShow()
{
    setEnabled(true);
    setMouseTracking(true);
    scene->clear();
    scene->addPixmap(m_image);
    scene->update();
    this->resetTransform();
    this->setSceneRect(m_image.rect());
    this->fitInView(QRect(0, 0, m_image.width(), m_image.height()), Qt::KeepAspectRatio);
}
void ImageViewer::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton){
        togglePan(true, event->pos());
        event->accept();
        return;
    }
    event->ignore();
}

void ImageViewer::mouseMoveEvent(QMouseEvent *event)
{
    if(m_isPan) {
        pan(event->pos());
        event->accept();
        return;
    }
    event->ignore();
}


void ImageViewer::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton){
        togglePan(false);
        event->accept();
        return;
    }
    event->ignore();
}

void ImageViewer::zoom(QPoint factor)
{
    QRectF FOV = this->mapToScene(this->rect()).boundingRect();
    QRectF FOVImage = QRectF(FOV.left(), FOV.top(), FOV.width(), FOV.height());
    float scaleX = static_cast<float>(m_image.width()) / FOVImage.width();
    float scaleY = static_cast<float>(m_image.height()) / FOVImage.height();
    float minScale = scaleX > scaleY ? scaleY : scaleX;
    float maxScale = scaleX > scaleY ? scaleX : scaleY;
    if ((factor.y() > 0 && minScale > 100) || (factor.y() < 0 && maxScale < 1 )) {
      return;
    }
    if(factor.y()>0)
        scale(1.2, 1.2);
    else
        scale(0.8, 0.8);
}
void ImageViewer::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_O) {
        this->resetTransform();
        this->setSceneRect(m_image.rect());
        this->fitInView(QRect(0, 0, m_image.width(), m_image.height()), Qt::KeepAspectRatio);
    }
}

void ImageViewer::pan(const QPoint &panTo)
{
    auto hBar = horizontalScrollBar();
    auto vBar = verticalScrollBar();
    auto delta = panTo - m_prevPan;
    m_prevPan = panTo;
    hBar->setValue(hBar->value() - delta.x());
    vBar->setValue(vBar->value() - delta.y());
}

void ImageViewer::wheelEvent(QWheelEvent *event)
{
    if(m_image.isNull())
        return;
    QPoint numDegrees = event->angleDelta() / 8;
    if (!numDegrees.isNull()) {
        QPoint numSteps = numDegrees / 15;
        zoom(numSteps);
    }
    event->accept();
}


void ImageViewer::togglePan(bool pan, const QPoint &startPos)
{
    if(pan){
        if(m_isPan) {
            return;
        }
        m_isPan = true;
        m_prevPan = startPos;
        setCursor(Qt::ClosedHandCursor);
    } else {
        if(!m_isPan) {
            return;
        }
        m_isPan = false;
        m_prevPan = QPoint(0,0);
        setCursor(Qt::ArrowCursor);
    }
}


/****************************************************************************\
 *
 * Jonathan Doman
 * 905260988
 * ECE 3574 - Homework 4
 * October 17, 2010
 *
\****************************************************************************/
#ifndef ABOUTWINDOW_H
#define ABOUTWINDOW_H

#include <QtGui/QDialog>
#include <QtGui/QLabel>
#include <QtGui/QGraphicsPixmapItem>
#include <QtGui/QCloseEvent>

// A dialog window for showing extra information
// about the project
class AboutWindow : public QDialog
{
    Q_OBJECT
public:
    explicit AboutWindow(QWidget *parent = 0);

public slots:
    void rotateHead();

signals:
   void dialogClosed();

protected:
   void closeEvent( QCloseEvent* e );

private:
    QGraphicsPixmapItem* _plassHead;
    QTimer* _rotateTimer;
    qreal _angle;
};

#endif // ABOUTWINDOW_H

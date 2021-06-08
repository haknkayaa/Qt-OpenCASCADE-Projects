/******************************************************************************
 *    ViewerActions.h 
 ******************************************************************************
 *     author      : Hakan Kaya, mail@hakankaya.kim
 *     date        : 2/19/21.
 *     description :
 ******************************************************************************/

#ifndef VIEWERACTIONS_H
#define VIEWERACTIONS_H

void MainWindow::viewTop() {
    myViewerWidget->viewTop();
}

void MainWindow::viewBottom() {
    myViewerWidget->viewBottom();
}

void MainWindow::viewRight() {
    myViewerWidget->viewRight();
}

void MainWindow::viewLeft() {
    myViewerWidget->viewLeft();
}

#endif //VIEWERACTIONS_H

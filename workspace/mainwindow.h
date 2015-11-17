#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QActionGroup>

#include "filemodel.h"
#include "fileview.h"
#include "trackmodel.h"
#include "trackview.h"

#include <cxxmidi/file.hpp>
#include <cxxmidi/output/default.hpp>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent_ = 0);
    ~MainWindow();

private slots:
    void onTrackSelected(QModelIndex index_);
    void onOpenFile();
    void onSaveAs();
    void onRequestAddTrack(int num_);
    void onRequestDeleteTrack(int num_);
    void onRequestClear(int num_);
    void onRequestAddEvent(int num_);
    void onRequestDeleteEvent(int num_);
    void onKeyChange(int key_);
    void onOutputSelected(QAction* action_);
    void setOutput(int num_);
    void eventsSelected(const QItemSelection& selected_,
                        const QItemSelection& deselected_);

private:

    void createMenu();

    Ui::MainWindow *_ui;
    QActionGroup * _outputsActionGroup;

    FileModel _fileModel;
    FileView _fileView;
    TrackModel _trackModel;
    TrackView _trackView;

    CxxMidi::File _file;
    CxxMidi::Output::Default* _midiOutput;
};

#endif // MAINWINDOW_H

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QHBoxLayout>
#include <QSplitter>
#include <QFileDialog>
#include <QApplication>

#include <cxxmidi/note.hpp>

MainWindow::MainWindow(QWidget *parent_)
    : QMainWindow(parent_)
    , _ui(new Ui::MainWindow)
    , _midiOutput(new CxxMidi::Output::Default(0))
{
    _ui->setupUi(this);

    if(QApplication::instance()->arguments().size() >=2 )
    {
        QString fileName =QApplication::instance()->arguments().at(1);
        _file = CxxMidi::File(fileName.toStdString().c_str());
    }

    _fileModel.setFile(&_file);
    _fileView.setModel(&_fileModel);

    connect(&_fileView,SIGNAL(activated(QModelIndex)),
            this,SLOT(onTrackSelected(QModelIndex)));
    connect(&_fileView,SIGNAL(clicked(QModelIndex)),
            this,SLOT(onTrackSelected(QModelIndex)));
    connect(&_fileView,SIGNAL(requestAddTrack(int)),
            this,SLOT(onRequestAddTrack(int)));
    connect(&_fileView,SIGNAL(requestDeleteTrack(int)),
            this,SLOT(onRequestDeleteTrack(int)));
    connect(&_fileView,SIGNAL(requestClear(int)),
            this,SLOT(onRequestClear(int)));

    connect(&_trackView,SIGNAL(requestAddEvent(int)),
            this,SLOT(onRequestAddEvent(int)));
    connect(&_trackView,SIGNAL(requestDeleteEvent(int)),
            this,SLOT(onRequestDeleteEvent(int)));

    _trackModel.setTrack(0);
    _trackView.setModel(&_trackModel);

    connect(_trackView.selectionModel(),SIGNAL(selectionChanged(const QItemSelection&,const QItemSelection&)),
            this,SLOT(eventsSelected(const QItemSelection&,const QItemSelection&)));

    QSplitter * splitter = new QSplitter;
    splitter->addWidget(&_fileView);
    splitter->addWidget(&_trackView);
    splitter->setStretchFactor(1,2);

    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(splitter);
    _ui->frame->setLayout(layout);

    this->onKeyChange(69);
    connect(_ui->spinBoxKey,SIGNAL(valueChanged(int)),
            this,SLOT(onKeyChange(int)));

    this->createMenu();

    // second argument is output num
    if(QApplication::arguments().size() >=3)
    {
        unsigned int num  = QApplication::arguments().at(2).toInt();
        if( num <_midiOutput->getPortCount() )
        {
            this->setOutput(num);
            _outputsActionGroup->actions()[num]->setChecked(true);
        }
    }
}

void MainWindow::eventsSelected(const QItemSelection& selected_,
                                const QItemSelection& /*deselected_*/)
{
    if(selected_.indexes().size() == 1)
    {
        int row =  selected_.indexes().at(0).row();
        CxxMidi::Track * track = _trackModel.track();
        CxxMidi::Event * event = &track->at(row);

        if(event->size() >=2)
        {
            int type = event->at(0);
            if( (type & 0xf0)  == CxxMidi::Event::NoteOn)
            {
                _midiOutput->reset();
                _midiOutput->sendMessage(event);
            }
        }
    }
}

void MainWindow::onKeyChange(int key_)
{
    std::stringstream ss;
    ss << "1st p. (straight): " << CxxMidi::Note::name(key_) << " / "
       << "2nd p. (cross): " << CxxMidi::Note::name(key_ + 7) << " / "
       << "3rd p. (slant): " << CxxMidi::Note::name(key_ + 14);

    _ui->labelKey->setText(ss.str().c_str());
    _trackModel.setKey(key_);
    _trackView.reset();
}

void MainWindow::createMenu()
{
    QMenu* fileMenu= this->menuBar()->addMenu(tr("&File"));

    QAction *action = fileMenu->addAction(tr("&Open"));
    connect(action,SIGNAL(triggered()),
            this,SLOT(onOpenFile()));

    action = fileMenu->addAction(tr("&Save as"));
    connect(action,SIGNAL(triggered()),
            this,SLOT(onSaveAs()));

    fileMenu->addSeparator();

    action = fileMenu->addAction(tr("&Exit"));
    QObject::connect(action,SIGNAL(triggered()),
                     QApplication::instance(),SLOT(quit()));
    //! @TODO ask if save changes

    // output menu
    QMenu* outputMenu= this->menuBar()->addMenu(tr("&Output"));
    _outputsActionGroup = new QActionGroup(this);
    _outputsActionGroup->setExclusive(true);

    QString name;
    for(size_t i=0; i<_midiOutput->getPortCount();i++)
    {
        name=QString("%1. %2").arg(i).arg(_midiOutput->getPortName(i).c_str());
        action = _outputsActionGroup->addAction(name);
        action->setCheckable(true);
        if(!i) // first is opened
            action->setChecked(true);
    }

    outputMenu->addActions(_outputsActionGroup->actions());

    connect(_outputsActionGroup,SIGNAL(triggered(QAction*)),
            this,SLOT(onOutputSelected(QAction*)));
}

void MainWindow::onOutputSelected(QAction* action_)
{
    int num = action_->text().split(".")[0].toInt();
    this->setOutput(num);
}

void MainWindow::setOutput(int num_)
{
    _midiOutput->openPort(num_);
}

void MainWindow::onRequestClear(int num_)
{
    _trackModel.setTrack(0);
    _fileModel.clearTrack(num_);
}

void MainWindow::onRequestAddTrack(int num_)
{
    //! @TODO there should be a command history (Undo/Redo)

    _trackModel.setTrack(0); // address may change
    _fileModel.addTrack(num_);
}

void MainWindow::onRequestDeleteTrack(int num_)
{
    //! @TODO there should be a command history (Undo/Redo)

    _trackModel.setTrack(0); // address may change
    _fileModel.removeTrack(num_);
}

void MainWindow::onRequestAddEvent(int num_)
{
    //! @TODO there should be a command history (Undo/Redo)

    _trackModel.addEvent(num_);
}

void MainWindow::onRequestDeleteEvent(int num_)
{
    //! @TODO there should be a command history (Undo/Redo)

    _trackModel.removeEvent(num_);
}

void MainWindow::onOpenFile()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Open file"), ".", tr("MIDI files (*.mid *.midi);;Any files (*)"));

    _file = CxxMidi::File(fileName.toStdString().c_str());
    _fileModel.setFile(&_file);
}

void MainWindow::onSaveAs()
{
    QFileDialog dialog(NULL, tr("Save As"));
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    dialog.setNameFilter(tr("MIDI files (*.mid *.midi);;Any files (*)"));
    if (dialog.exec())
    {
        QString fileName = dialog.selectedFiles().at(0);
        if(QFileInfo(fileName).suffix().isEmpty())
            fileName += ".mid";

        //! @TODO ask if overwrite

        _file.saveAs(fileName.toStdString().c_str());
    }
}

void MainWindow::onTrackSelected(QModelIndex index_)
{
    _trackModel.setTrack(&_file.at(index_.row()));
    _trackView.setModel(&_trackModel);
}

MainWindow::~MainWindow()
{
    delete _ui;
}
